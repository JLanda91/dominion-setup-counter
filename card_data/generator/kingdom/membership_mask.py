import cog

from generator.util import lower_snake_case
import generator.kingdom_venn_region_data

class KingdomMembershipMaskGenerator:
    def __init__(self, venn_region_data: generator.kingdom_venn_region_data.KingdomVennRegionData):
        self._venn_region_data = venn_region_data
        self._predicates = self._venn_region_data.code_generator_config.supply_kingdom_venn_predicates

    def generate_members(self):
        for venn_predicate in self._predicates:
            cog.outl(f"bool is_{lower_snake_case(venn_predicate.name)}_region : 1;")
        cog.outl()
        cog.outl("static constexpr MembershipMask FromUnsigned(uint8_t bit_pack) noexcept {")
        cog.outl("\treturn MembershipMask{")
        for i, venn_predicate in enumerate(self._predicates):
            cog.outl(f"\t\t.is_{lower_snake_case(venn_predicate.name)}_region = (bit_pack & (1u << {i}u)) > 0{',' if i < len(self._predicates) - 1 else ''}")
        cog.outl("\t};")
        cog.outl("}")
        cog.outl()
        cog.outl("static constexpr uint8_t ToUnsigned(const MembershipMask& m) noexcept {")
        cog.outl("\treturn")
        for i, venn_predicate in enumerate(self._predicates):
            cog.outl(f"\t\t(m.is_{lower_snake_case(venn_predicate.name)}_region << {i}uz) {'+' if i < len(self._predicates) - 1 else ';'}")
        cog.outl("}")

    def generate_num_members(self):
        cog.outl(f"inline constexpr std::size_t kNumMembershipRegions = {len(self._predicates)}uz;")

instance = KingdomMembershipMaskGenerator(generator.kingdom_venn_region_data.instance)
