# Dominion Setup Counter
Counts the number of 2-player Dominion setups using CUDA.

## Three phases
We only consider card types and other attributes that directly influence the setup of the game to simplify computation.

### Kingdom Phase
Kingdom cards have two attributes:
- One or more **Types**.
- A **Cost**: 1-8 or P (potions). 

We try to group the 498 cards in as few buckets as possible, according to what makes the cards in them unique to the
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
  - Loot: can also be entered by Events (Setup Landscapes) and a Trait (Cursed). There are 2 each of 15 unique cards. All 30 are shuffled, giving (30!)/(2!)^15 = 8094874872198213459360000000 combinations.
  - Young Witch: add an *unused* kingdom card to the kingdom with a cost of 2 or 3 to form a pile of Bane cards. This is added to the supply.
  - Knights: the pile of 10 different knights is added.
  - Druid: it is also a Fate card but a special case. After adding the Boons, set aside the top 3 Boons face up. These will only be obtainable with the Druid. Any other Fate cards in play only draw from the 9 other Boons. Still 12! combinations
  - Ferryman: add an *unused* kingdom card costing 3 or 4. This is *not* added to the supply.
  - Riverboat: add an *unused* kingdom card of Action but not Duration type costing 5. This is *not* added to the supply.

You might wonder why there is no Action or Duration in this list. This is completely taken caren of by the next bucket property.

Secondly, the Extra Setup phase requires to pick some extra cards which could trigger more special setup 
behavior. We therefore create a Venn Diagram and divide the cards into regions, each uniquely determined by which 
special setup cards they can possibly be chosen. There are 7 predicates determining a region. For a total of 2^7 = 128
possible regions. However only a few of them actually contain the cards. We can encode the region by setting 7 bits in a
number to 0 or 1, each corresponding to the predicate outcome. Denote this as the **Venn Diagram Region Mask (VDRM)**.

These two properties, **Effective Type** and **VDRM** will uniquely determine a bucket.

### Landscape Phase


### Extra Setup Phase
In order, handle the following extra setup cards in order:
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
