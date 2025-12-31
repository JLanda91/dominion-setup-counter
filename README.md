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

Secondly, the Extra Setup phase requires to pick some extra cards which could trigger more special setup
behavior. We therefore create a Venn Diagram and divide the cards into regions, each uniquely determined by which
special setup cards they can possibly be chosen. There are 6 predicates determining a region. For a total of 2^6 = 64
possible regions. However, only 15 of them actually contain the cards. We can encode the region by setting 6 bits in a
number to 0 or 1, each corresponding to the predicate outcome. Denote this as the **Venn Diagram Region Mask (VDRM)**.

These two labels, **Effective Type** and **VDRM** will uniquely determine an equivalence class. 
The equivalence class has a size, denoting how many of the 498 cards belong to that equivalence class.
The equivalence class also has an amount not greater than the size, denoting the number of action or treasure cards.
This is needed for the Landscape Phase.

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

The VDRM of a kingdom card is determined by the following 6 predicates. Is it: 
1. Pickable by Young Witch (if unused and added to supply): costing 2 or 3?
2. Pickable by Approaching Army (if unused and added to supply): is Action?
3. Pickable by Way of the Mouse (if unused): non-Duration (erratum) Action and costing 2 or 3?
4. Pickable by Ferryman (if unused): costing 3 or 4?
5. Pickable by Riverboat (if unused): non-Duration Action card costing 5?
6. Pickable by Obelisk (if in supply): is Action?

You can see that some predicates have overlap with, are identical to, or are a subset of other predicates. For example
the predicate for Obelisk and Approaching Army is the same, but their choices will never overlap as the former picks a supply
pile and the latter an unused pile. 

It might occur that one of these extra setup handling steps introduces an **Effective Type** that was not previously in 
play. It is well possible that a kingdom supply without Fate still gets Fate through any of these steps, in which case
also the Boons are added.

## The algorithm

### Kingdom Phase
Given the current card data, there are 48 equivalence classes. The sum over the equivalence class sizes is of course 498.
The sum over the equivalence classes number of action or treasure cards is 475.
This way we only have to iterate over all the ways to distribute 10 cards over these equivalence classes (N =~ 1.7e10 ways)
instead of iterating over all ways ((498 c 10) =~ 2.36e20) to pick kingdom supplies.

#### Iterating over Equivalence Class Distributions (ECDs)
Since the equivalence class sizes are known up front (compile-time) we can also make a compile-time table that can 
translate an index n in [0, N) to an actual list of amounts for each equivalence class (an ECD) to enable embarrassingly
parallel work on the GPU. Let this index be called the ECD index. The translation is made such that the last Equivalence
Classes in the list gets assigned big numbers first and the higher the ECD Index the more the first Equivalence Classes
in the list get assigned high amounts of cards.

#### Obtaining useful data from the ECD
For subsequent phases it is handy to derive from the ECD:
- For each ET if it is already in play at that point (12-bit mask).
- How many ways we can obtain 0 through 10 Action or Treasure cards with this ECD. The sum of this array must equal the binomial product of (Size c Amount) over the EC

### Landscape Phase
We could work and iterated with Equivalence Classes here, distinguishing on the cards that change the downstream 
behavior:
- Obelisk: 1 card
- Way of the Mouse: 1 card
- Cursed: 1 card
- Other Traits (not Curse): 14 cards
- Loot-inducing Events: 5 cards
- Others: 133 (20 non-Obelisk Landmarks, 19 non-Mouse Ways, 20 Projects, 74 other Events)

#### Something more optimal than iterating over all ECDs
There would be only 25 ECDs in this case, but this phase really can only introduce the Loot pile (if not already setup
by the Kingdom supply), Obelisk and Way of the Mouse as differentiable behavior. So there are only 2^3=8 cases to 
differentiate. Of course these are dependent whether Loot was already entered by the Kingdom phase, and if not it can be 
entered either by the Events that introduce Loot, or by the Cursed Trait, which not only needs to be picked, but there 
will also have to be enough Action or Treasure cards in the Kingdom supply to assign Cursed to. Depending on how many
Action/Treasure cards the kingdom has the amount of ways to assign the Traits to these vary.

The below tables have the coefficients for each 8 possibilities. The coefficents denote how many ways there are to
deal a maximum of 2 landscape cards for the given mutation, the mutation being: Loot pile, Way of the Mouse and Obelisk
in play after this phase.

If the kingdom ECD did not introduce Loot.

| Number of A/T cards in Kingdom ECD    | 0     | 1     | K (2+)                  |
|---------------------------------------|-------|-------|-------------------------|
| No Loot                               | 11027 | 10984 | 8912 + 1876K + 91K(K-1) |
| No Loot, Way of the Mouse             | 149   | 148   | 134 + 14K               |
| No Loot, Obelisk                      | 149   | 148   | 134 + 14K               |
| No Loot, Obelisk and Way of the Mouse | 1     | 1     | 1                       |
| Loot                                  | 755   | 903   | 680 + 209K + 14K(K-1)   |
| Loot, Way of the Mouse                | 5     | 6     | 5 + K                   |
| Loot, Obelisk                         | 5     | 6     | 5 + K                   |

If the kingdom ECD already introduced Loot:

| Number of A/T cards in Kingdom ECD | 1     | K (2+)                   |
|------------------------------------|-------|--------------------------|
| Loot                               | 11887 | 9592 + 2085K + 105K(K-1) |
| Loot, Way of the Mouse             | 154   | 139 + 15K                |
| Loot, Obelisk                      | 154   | 139 + 15K                |
| Loot, Obelisk and Way of the Mouse | 1     | 1                        |

The Excel sheet 'Landscape Coefficients' in the repository has the tables as well.
For the keen observer: the coefficients are symmetric when there is either Obelisk or Way of the Mouse.
It is impossible to have 0 number of Action or Treasure cards already in de kingdom ECD and also having introduced the
Loot pile at this point as all GainsLoot cards are Action or Treasure cards.

To optimize this phase, we should use the size 11 array from the Kingdom Phase containing the amount of ways to 
obtaining K Action or Treasure cards from the ECD. We then inner product these values with the row in the table to get
one coefficient for a specific mutation.

**Example**: given an ECD the amount of ways to get K Action/Treasure cards from the ECD is denoted by the K-indexed array

{12, 49, 2490, 12540, 9350, 2359, 219354, 9235, 38, 39210, 92}. 

Assume the ECD already introduced the Loot pile and 
assume we are interested in how many ways we enter the Extra Setup Phase with only the Loot mutation.
This coefficient will equal the dot product of the aforementioned array with the array 

{0, 11887, 13972, 16477, 19192, 22117, 25252, 28597, 32152, 35917, 39892}

which equals 7690031439. In short: the amount of ways, given the ECD, to get a specific mutation in the Landscape Phase
can be calculated up front. This saves calls to the Extra Setup Phase.

Doing this for all mutations gives a small matrix-vector product of size 5 x 11 for ECDs without GainsLoot cards,
and 3 x 11 for ECDs with GainsLoot cards.

#### Returning from this phase
This phase is, all concluded, fairly easy. Given if the kingdom ECD has the Loot pile already set up, and given the
amount of Action/Treasure cards in it, the only data this phase should return is the inner product for each mutation. There are
5 unique coefficients for the No-Loot-In-Kingdom-ECD table and 3 for the others. And there will be
7 and 4 calls to the extra setup phase respectively.

### Extra Setup Phase
Since this phase can introduce other cards, we need to know exactly:
- The kingdom ECD (returned from the Kingdom phase).
- Which Kingdom Effective Types are already in play.
- Which Extra Setup cards are already in play. 

This phase is, algorithmically the most complex part as it contains recursive logic. After all we need to handle the Extra Setup 
Card Handling Order (ESCHO) Because in general the outline is:
- Determine the first Extra Setup card to handle from the ESCHO.
- For all ECs in the ECD that have a VDRM matching this card:
  - mark the index of this EC for this extra setup card,
  - multiply the amount of ways there are to choose a card from this EC Amount (from supply) or from EC MaxAmount - Amount (unused),
  - multiply possible factors (for example if Fate is introduced, multiply an extra 12!)
  - Add a possible extra added Extra Setup Card to the order
- Recurse.

There is a nasty edge case where we enter Omen in this phase, because then there are two branches to take:
- 14 * the path without Approaching Army
- 1 * the path with Approaching Army added to the handling order.

which adds to the runtime complexity.

The GPU, however, cannot handle recursion well so we will hand-roll a stack-based while-loop approach.
We will have to hand-roll a stack of static capacity 6 (after all there are not more than 6 Extra Setup Cards), that
will hold the recursion context, which will have to contain:
- The current Extra Setup card (as bit index of the VDRM). Values: 0-5 (1 byte).
- The mask of the remaining ESCHO (6-bit mask, 1 byte). This is needed if an extra setup card is chosen in this phase, to preserve the knowledge of the mutated order.
- The EC index marking the choice for this extra setup cards (1 byte).
- The return value for this sub-tree (32 bytes)

which together is aligned to 64 bytes. The ContextStack therefore is 384 bytes.

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
