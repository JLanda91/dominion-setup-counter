import cog
from math import ceil

from generator.util import lower_snake_case, pascal_case
import generator.kingdom_venn_region_data as kingdom_venn_region_data

class KingdomCardTypeTallyGenerator:
    def __init__(self, venn_region_data: kingdom_venn_region_data.KingdomVennRegionData):
        self._venn_region_data = venn_region_data

        max_dict = self._venn_region_data.data.groupby("Card Type")["Count"].max().to_dict()
        bit_size_dict = dict((k, len(bin(v)[2:])) for k, v in max_dict.items())
        type_str_dict = dict((k, f"uint{8*(int(ceil(v/8)))}_t") for k, v in bit_size_dict.items())
        self._predicates = self._venn_region_data.code_generator_config.supply_kingdom_card_type_predicates
        self._card_category_bit_sizes =  dict((k, {"bit_size": bit_size_dict[k], "type_str": type_str_dict[k]}) for k in max_dict)

    def generate_members(self, bitpack_members = False):
        for card_type_predicate in self._predicates:
            bit_size_data = self._card_category_bit_sizes[card_type_predicate.name]
            if bitpack_members:
                cog.outl(f"{bit_size_data["type_str"]} amount_{lower_snake_case(card_type_predicate.name)} : {bit_size_data["bit_size"]};")
            else:
                cog.outl(f"{bit_size_data["type_str"]} amount_{lower_snake_case(card_type_predicate.name)};")

    def generate_num_members(self):
        cog.outl(f"inline constexpr std::size_t kNumCardTypes = {len(self._card_category_bit_sizes)}uz;")

    def generate_mutable_get(self):
        cog.outl("""
            template<std::size_t N>
            constexpr auto& card_type_amount(CardTypeTally& tally) noexcept {
                static_assert(N < kNumCardTypes, "Index out of bounds");""", dedent=True, trimblanklines=True)
        self._generate_member_get()
        cog.outl("}")

    def generate_const_get(self):
        cog.outl("""
            template<std::size_t N>
            constexpr const auto& card_type_amount(const CardTypeTally& tally) noexcept {
                static_assert(N < kNumCardTypes, "Index out of bounds");""", dedent=True, trimblanklines=True)
        self._generate_member_get()
        cog.outl("}")

    def generate_member_name(self):
        cog.outl("""
            template<std::size_t N>
            constexpr auto card_type_name() noexcept -> std::string_view {
                static_assert(N < kNumCardTypes, "Index out of bounds");""", dedent=True, trimblanklines=True)
        for i, card_type_predicate in enumerate(self._predicates):
            cog.out("\t")
            if i > 0:
                cog.out("else ")
            if i < len(self._predicates) - 1:
                cog.out(f"if constexpr (N == {i}) ")
            cog.outl(f"{{ return \"{card_type_predicate.name}\"; }}")
        cog.outl("}")

    def generate_total(self):
        cog.out(f"""
            constexpr auto card_type_total(const CardTypeTally& tally) noexcept -> uint8_t {{
                return """, dedent=True, trimblanklines=True)
        for i, card_type_predicate in enumerate(self._predicates):
            cog.outl(f"""\t\ttally.amount_{lower_snake_case(card_type_predicate.name)} {'+' if i < len(self._predicates) - 1 else ';'}""")
        cog.outl("}")

    def generate_enum_class(self):
        cog.outl("enum class CardType : uint8_t {")
        for i, card_type_predicate in enumerate(self._predicates):
            cog.outl(f"\t{pascal_case(card_type_predicate.name)} = {i},")
        cog.outl("};")
        
    def _generate_member_get(self):
        for i, card_type_predicate in enumerate(self._predicates):
            cog.out("\t")
            if i > 0:
                cog.out("else ")
            if i < len(self._predicates) - 1:
                cog.out(f"if constexpr (N == {i}) ")
            cog.outl(f"{{ return tally.amount_{lower_snake_case(card_type_predicate.name)}; }}")

instance = KingdomCardTypeTallyGenerator(kingdom_venn_region_data.instance)