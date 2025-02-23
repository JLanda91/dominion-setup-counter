import cog

from generator.util import lower_snake_case, pascal_case
import generator.kingdom_venn_region_data as kingdom_venn_region_data

class CardTypeMajorTableGenerator:
    def __init__(self, venn_region_data: kingdom_venn_region_data.KingdomVennRegionData):
        self._venn_region_data = venn_region_data
        self._predicates = self._venn_region_data.code_generator_config.supply_kingdom_card_type_predicates
        self._num_card_types = self._venn_region_data.data["Card Type"].nunique()
        self._num_venn_regions = self._venn_region_data.data["Venn Mask"].nunique()

    def generate_card_type_enum_class(self):
        cog.outl("enum class CardType : uint8_t {")
        for i, card_type_predicate in enumerate(self._predicates):
            cog.outl(f"\t{pascal_case(card_type_predicate.name)} = {i},")
        cog.outl("};")
        cog.outl()
        cog.outl(f"inline constexpr std::array<std::string_view, {len(self._predicates)}uz> kCardTypeNames = {{ {", ".join(f"\"{pascal_case(card_type_predicate.name)}\"" for card_type_predicate in self._predicates)} }};")

    def generate_size_constants(self):
        cog.outl(f"inline constexpr std::size_t kNumVennRegions = {self._num_venn_regions}uz;")
        cog.outl(f"inline constexpr std::size_t kNumCardTypes = {self._num_card_types}uz;")

    @staticmethod
    def generate_sized_alias():
        cog.outl(f"using card_type_major_table_t = ColumnMajorTable<kNumVennRegions, kNumCardTypes>;")

    def generate_singleton(self):
        card_types = list(p.name for p in self._venn_region_data.code_generator_config.supply_kingdom_card_type_predicates)
        membership_masks = sorted(self._venn_region_data.data["Venn Mask"].unique().tolist())
        data_dict = self._venn_region_data.data.set_index(["Venn Mask", "Card Type"])["Count"].to_dict()

        membership_masks_initializer = ",".join(f"MembershipMask::FromUnsigned({membership_mask}u)" for membership_mask in membership_masks)
        data_initializer = ",".join(str(data_dict.get((membership_mask, card_type), 0)) for card_type in card_types for membership_mask in membership_masks)

        cog.outl(f"""
            inline constexpr auto card_type_major_table() -> const card_type_major_table_t& {{
                static constexpr card_type_major_table_t singleton = []{{
                    const card_type_major_table_t::row_label_t row_labels {{ {membership_masks_initializer} }};
                    const card_type_major_table_t::data_t data {{ {data_initializer} }} ;
                    return card_type_major_table_t{{row_labels, data}} ;
                }}();
                return singleton;
            }}
        """, dedent=True, trimblanklines=True)

instance = CardTypeMajorTableGenerator(kingdom_venn_region_data.instance)