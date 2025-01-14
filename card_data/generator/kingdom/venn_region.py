import cog
import generator.kingdom_venn_region_data


class VennRegionGenerator:
    def __init__(self, venn_region_data: generator.kingdom_venn_region_data.KingdomVennRegionData):
        self._venn_region_data = venn_region_data

    def generate_singleton(self):
        card_types = list(p.name for p in self._venn_region_data.code_generator_config.supply_kingdom_card_type_predicates)
        membership_masks = sorted(self._venn_region_data.data["Venn Mask"].unique().tolist())
        tally_dict = self._venn_region_data.data.set_index(["Venn Mask", "Card Type"])["Count"].to_dict()

        cog.outl(f"inline constexpr auto kKingdomVennRegionsSize = {len(membership_masks)}uz;")
        cog.outl(f"using kingdom_venn_region_array_t = std::array<VennRegion, kKingdomVennRegionsSize>;")
        cog.outl()
        cog.outl("inline constexpr kingdom_venn_region_array_t kKingdomVennRegions {{")
        for membership_mask in membership_masks:
            cog.out(f"\t{{ MembershipMask::FromUnsigned({membership_mask}u), CardTypeTally{{")
            cog.out(f"{', '.join(str(tally_dict.get((membership_mask, card_type), 0)) for card_type in card_types)}")
            cog.outl("} },")
        cog.outl("}};")
        cog.outl()
        cog.outl("static_assert(std::ranges::fold_left(kKingdomVennRegions, 0u, [](auto acc, const auto& elem){")
        cog.outl("\treturn acc + card_type_total(elem.card_type_tally);")
        cog.outl(f"}}) == {self._venn_region_data.data['Count'].sum()}u, \"Card count mismatch\");")


instance = VennRegionGenerator(generator.kingdom_venn_region_data.instance)