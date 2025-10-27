# Dominion Setup Counter
Counts the number of 2-player Dominion setups using CUDA.

## Three phases
We choose the kingdom supply first (10 piles), then choose not more than 2 supply landscapes, and then do the extra 
setup required by some of the cards that may introduce it.

### Kingdom Phase
Kingdom cards have two attributes:
- One or more **Types**.
- A **Cost**: 1-8, P (potions). 

We try to group the 498 cards in as few equivalence classes as possible, according to what makes the cards in them unique to the
computation. 

Firstly, some **Types** are not relevant to the setup, e.g., Victory, Reserve, Night, etc. We will only distinguish on 
those that affect the setup computation. Some of them are single special cards that have some unique behavior. We
therefore introduce the **Effective Type (ET)**:
- None: default category if it doesn't belong to another ET.
- Looter: add 10 (for 2 players) Ruins cards. There are 9765625 ways to select 10 cards from 5 sets, each containing 10 of the same card. This is *not* an extra kingdom pile.
- Fate: add and shuffle the 12 Boon cards (12! = 479001600 ways). This is *not* added to the supply.
- Doom: add and shuffle the 12 Hex cards (12! = 479001600 ways). This is *not* added to the supply.
- Liaison: add one of the 23 Ally cards. This is *not* one of the Setup Landscapes.
- Omen: add one of the 15 Prophecy cards. This is *not* one of the Setup Landscapes.
- GainsLoot: add a pile of Loot cards. This pile can also be entered by Events (Setup Landscapes) and a Trait (Cursed). There are 2 each of 15 unique Loot cards. All 30 are shuffled, giving (30!)/(2!)^15 = 8094874872198213459360000000 combinations.
- Young Witch: add an *unused* kingdom card to the kingdom with a cost of 2 or 3 to form a pile of Bane cards. This is added to the supply.
- Knights: the pile of 10 different knights is added.
- Druid: it is also a Fate card but a special case. After adding the Boons, set aside the top 3 Boons face up. These will only be obtainable with the Druid. Any other Fate cards in play only draw from the 9 other Boons. Still 12! combinations
- Ferryman: add an *unused* kingdom card costing 3 or 4. This is *not* added to the supply.
- Riverboat: add an *unused* kingdom card of Action but not Duration type costing 5. This is *not* added to the supply.

You might wonder why there is no Action or Duration in this list. This is completely taken caren of by the next equivalence class property.

Secondly, the Extra Setup phase requires to pick some extra cards which could trigger more special setup 
behavior. We therefore create a Venn Diagram and divide the cards into regions, each uniquely determined by which 
special setup cards they can possibly be chosen. There are 7 predicates determining a region. For a total of 2^7 = 128
possible regions. However only a few of them actually contain the cards. We can encode the region by setting 7 bits in a
number to 0 or 1, each corresponding to the predicate outcome. Denote this as the **Venn Diagram Region Mask (VDRM)**.

These two properties, **Effective Type** and **VDRM** will uniquely determine an equivalence class. The equivalence class has a size, denoting how many of the 498 cards belong to that equivalence class.

### Landscape Phase
We choose not more than two cards from the joint set of Events, Landmarks, Projects, Ways and Traits. This would be an 
easy phase were it not for the Traits and three special cards.

Firstly, if there are any Traits in the picked Landscapes, they must each be put under a different Action or Treasure kingdom supply pile.
There are therefore some distinguished scenarios:
- At least as many Action or Treasure piles (n) as Traits (k): divide them. (n!)/(n-k)! possibilities.
- No Action or Treasure piles (highly unlikely): 1 possibility, being none of the Traits used.
- One Action or Treasure pile and 2 Traits: 2 possibilities. Choose one of the two that goes under the pile.

Secondly, there are three special cards in this phase that modify behavior.
- Way of the Mouse: add a non-Duration (erratum) Action card costing 2 or 3 in the Extra Setup Phase. This is *not* added to the kingdom supply but counts as a separate pile.
- Obelisk: choose a kingdom supply pile which are Action in the Extra Setup Phase.
- Cursed: the implied behavior is as the name of the card. It is a Trait, but it also introduces Loot so if not already done, the Loot pile must be added.

This brings a branch: if we enter this phase without the Loot pile, picking Curse and being able to assign it to a kingdom pile
will add the Loot pile. In this case we also want to know how many Action or Treasure cards we have and if that amount is more or 
less than the amount of Traits we picked because Cursed might not be able to be assigned to a pile. If we enter this 
phase already with the Loot pile, Curse doesn't add anything. This is important to the algorithm and its optimization. 

### Extra Setup Phase
Handle the following extra setup cards in order if they are in play:
1. Young Witch
2. Approaching Army
3. Way of the Mouse
4. Ferryman
5. Riverboat
6. Obelisk

It is possible that one extra setup cards introduces another extra setup card into the game. If so, handle that one
according to the order above. That new extra setup card might be handled before the remainder of the setup cards already
waiting to be handled. For example if Riverboat and Obelisk are handled and Riverboat chooses Ferryman, Ferryman is 
handled before Obelisk.

The VDRM of a kingdom card is determined by the following 7 predicates. Is it: 
1. Pickable by Young Witch (if unused and added to supply): costing 2 or 3?
2. Pickable by Approaching Army (if unused and added to supply): is Action?
3. Pickable by Way of the Mouse (if unused): non-Duration (erratum) Action and costing 2 or 3?
4. Pickable by Ferryman (if unused): costing 3 or 4?
5. Pickable by Riverboat (if unused): non-Duration Action card costing 5?
6. Pickable by Obelisk (if in supply): is Action?
7. Action or Treasure (or both)? (used for Cursed, see Landscape Phase)

You can see that some predicates have overlap with, are identical to, or are a subset of other predicates. For example
the predicate for Obelisk and Approaching Army is the same, but their choices will never overlap as the former picks a supply
pile and the latter an unused pile. 

It might occur that one of these extra setup handling steps introduces an **Effective Type** that was not previously in 
play. It is well possible that a kingdom supply without Fate still gets Fate through any of these steps, in which case
also the Boons are added.

## The algorithm

### Kingdom Phase
Given the current card data, there are 53 equivalence classes. The sum over the equivalence class sizes is of course 498.
This way we only have to iterate over all the ways to distribute 10 cards over these equivalence classes (N =~ 4.9e10 ways)
instead of iterating over all ways ((498 c 10) =~ 2.36e20) to pick kingdom supplies.

#### Iterating over Equivalence Class Distributions (ECDs)
Since the equivalence class sizes are known up front (compile-time) we can also make a compile-time table that can 
translate an index n in [0, N) to an actual list of amounts for each equivalence class (an ECD) to enable embarrassingly
parallel work on the GPU. Let this index be called the ECD index. The translation is made such that the last Equivalence
Classes in the list gets assigned big numbers first and the higher the ECD Index the more the first Equivalence Classes
in the list get assigned high amounts of cards.

#### Obtaining useful data from the ECD
For subsequent phases it is handy to derive from the ECD at least for each ET if it is already in play at that point.
For the Landscape phase we also want to sum the EC sizes for ECs that are Action or Treasure cards (Predicate 7 of the
VDRM). We take this information along to the next phase.

#### Ending the phase
When the phases have been completed we only need to multiply the result by taking the product of (MaxAmount c Amount)
over all ECs as this effectively deduplicates for all possible ways to choose which cards in the ECs are in play.

### Landscape Phase
We could work and iterated with Equivalence Classes here, distinguishing on the cards that change the downstream 
behavior:
- Obelisk: 1 card
- Way of the Mouse: 1 card
- Cursed: 1 card
- Other Traits (not Curse): 14 cards
- Loot-inducing Events: 5 cards
- Others: 133 (20 non-Obelisk Landmarks, 19 non-Mouse Ways, 20 Projects, 74 other Events)

There would be only 25 ECDs in this case, but this phase really can only introduce the Loot pile (if not already setup
by the Kingdom supply), Obelisk and Way of the Mouse as differentiable behavior. So there are only 2^3=8 cases to 
differentiate. Of course these are dependent whether Loot was already entered by the Kingdom phase, and if not it can be 
entered either by the Events that introduce Loot, or by the Cursed Trait, which not only needs to be picked, but there 
will also have to be enough Action or Treasure cards in the Kingdom supply to assign Cursed to. Depending on how many
Action/Treasure cards the kingdom has the amount of ways to assign the Traits to these vary.

The below tables have the coefficients for each 8 possibilities.

If the kingdom ECD did not introduce Loot.

| Number of A/T cards in Kingdom ECD    | 0     | 1     | N (2+)                  |
|---------------------------------------|-------|-------|-------------------------|
| No Loot                               | 11027 | 10984 | 8912 + 1876N + 91N(N-1) |
| No Loot, Way of the Mouse             | 149   | 148   | 134 + 14N               |
| No Loot, Obelisk                      | 149   | 148   | 134 + 14N               |
| No Loot, Obelisk and Way of the Mouse | 1     | 1     | 1                       |
| Loot                                  | 755   | 903   | 680 + 209N + 14N(N-1)   |
| Loot, Way of the Mouse                | 5     | 6     | 5 + N                   |
| Loot, Obelisk                         | 5     | 6     | 5 + N                   |

If the kingdom ECD already introduced Loot:

| Number of A/T cards in Kingdom ECD | 1     | N (2+)                   |
|------------------------------------|-------|--------------------------|
| Loot                               | 11887 | 9592 + 2085N + 105N(N-1) |
| Loot, Way of the Mouse             | 154   | 139 + 15N                |
| Loot, Obelisk                      | 154   | 139 + 15N                |
| Loot, Obelisk and Way of the Mouse | 1     | 1                        |

The excel sheet Landscape Coefficients in the repository has the tables as well.


### Extra Setup Phase

## Code Optimizations
### Optimally ordering the list of ECs
To optimize for SIMD execution of the GPU warps, we want to have "near equal thread id = near equal code paths taken".
We will therefore analyze which part of the program will most likely have the most divergent branches. This is most likely
the Extra Setup Phase as the set of Extra Setup Cards will greatly contribute to how many of them will be handled and in which order.
The Landscape phase also has one simple branch as describe before.

We choose to put the **Effective Types** of the Equivalence Classes that are linked to the Extra Setup phase to the front:
Young Witch, Ferryman and Riverboat. Since Approaching Army can only be in play if there is at least one EC with ET=Omen
that has a nonzero amount, we put the Omen ECs next, and then the GainsLoot ECs to guarantee that the bursts of ECD indices
containing near-similar behavior for the Extra Setup Phase and subsequently the Landscape Phase are coalesced. The other
ECs have ETs that don't change the code paths but will once multiply a number to the outcome.

### Templating away possible branches
Instead of letting one kernel iterate over all ECD indices and doing the work, we want to optimize away divergent branching.
We can template our kernel on which of the 2^6 = 64 subsets of Extra Setup Cards will be in play when arrive at that phase.
This will also reduce the code for the Kingdom Phase and Landscape Phase as for the former, the amounts for the 
Young Witch, Ferryman and Riverboat EC will be fixed. For the latter, Obelisk and Way of the Mouse are fixed such that
the space becomes less big.

**Note**: if Approaching Army is set to ON in this template parameter, we must only iterate over the ECD Indexes corresponding to
ECDs that has at least one EC with ET=Omen with a nonzero amount. If not we can iterate over both possibilities as it is
possible to have Omen in play but not Approaching Army.
