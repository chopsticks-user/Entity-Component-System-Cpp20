#include "TestUtils.hpp"

// #include "Container/PolyTypeTable.hpp"

using ushi::internal::container::PolyTypeTable;
using ushi::internal::container::VectorWrapper;
using ushi::internal::container::VectorWrapperBase;

using TTable =
    std::unordered_map<std::type_index,
                       std::shared_ptr<VectorWrapperBase<ushi::Component>>>;

TEST_CASE("Case #01: Guaranteed behavirors", "[require]") {
  TTable t1{};
  t1[typeid(Motion)] =
      std::make_shared<VectorWrapper<Motion, ushi::Component>>();
  t1[typeid(Animation)] =
      std::make_shared<VectorWrapper<Animation, ushi::Component>>();
  t1[typeid(Texture)] =
      std::make_shared<VectorWrapper<Texture, ushi::Component>>();

  auto table1 = PolyTypeTable<ushi::u64, ushi::Component>{t1};
  REQUIRE(table1.nTypes() == 3);

  TTable t2{};
  t2[typeid(Motion)] =
      std::make_shared<VectorWrapper<Motion, ushi::Component>>();
  t2[typeid(Animation)] =
      std::make_shared<VectorWrapper<Animation, ushi::Component>>();
  t2[typeid(Texture)] =
      std::make_shared<VectorWrapper<Texture, ushi::Component>>();
  t2[typeid(Audio)] = std::make_shared<VectorWrapper<Audio, ushi::Component>>();

  auto table2 = PolyTypeTable<ushi::u64, ushi::Component>{t2};
  REQUIRE(table2.nTypes() == 4);

  table1.addWithTypes<Motion, Animation, Texture>(5, {10}, {15}, {20});
  table1.addWithTypes<Motion, Animation, Texture>(9, {18}, {27}, {36});
  REQUIRE(table1.size() == 2);
  REQUIRE(table1.validate());

  table2.addWithTypes(7, Audio{14}, Motion{21}, Animation{28}, Texture{35});
  table2.addWithTypes(11, Audio{22}, Motion{33}, Animation{44}, Texture{55});
  REQUIRE(table2.size() == 2);
  REQUIRE(table2.validate());

  auto package = table2.popAt(7);
  REQUIRE(table2.size() == 1);
  REQUIRE(table2.validate());
  REQUIRE_FALSE(table2.contains(7));

  package.erase(typeid(Audio));
  table1.addWithPackage(7, package);
  REQUIRE(table1.size() == 3);
  REQUIRE(table1.validate());
  REQUIRE(table1.contains(7));

  REQUIRE(table1.at<Motion>(7).value == 21);
  REQUIRE(table1.at<Animation>(7).value == 28);
  REQUIRE(table1.at<Texture>(7).value == 35);

  package = table1.popAt(5);
  REQUIRE(table1.size() == 2);
  REQUIRE(table1.validate());
  REQUIRE_FALSE(table1.contains(5));

  package[typeid(Audio)] = std::make_unique<Audio>(25);
  table2.addWithPackage(5, package);
  REQUIRE(table2.size() == 2);
  REQUIRE(table2.validate());
  REQUIRE(table2.contains(5));

  REQUIRE(table2.at<Motion>(5).value == 10);
  REQUIRE(table2.at<Animation>(5).value == 15);
  REQUIRE(table2.at<Texture>(5).value == 20);
  REQUIRE(table2.at<Audio>(5).value == 25);

  package[typeid(Motion)] = std::make_unique<Motion>(1);
  package[typeid(Animation)] = std::make_unique<Animation>(2);
  package[typeid(Texture)] = std::make_unique<Texture>(3);
  package[typeid(Audio)] = std::make_unique<Audio>(4);

  table2.addWithPackage(5, package);

  REQUIRE(table2.size() == 2);
  REQUIRE(table2.validate());
  REQUIRE(table2.contains(5));

  REQUIRE(table2.at<Motion>(5).value == 1);
  REQUIRE(table2.at<Animation>(5).value == 2);
  REQUIRE(table2.at<Texture>(5).value == 3);
  REQUIRE(table2.at<Audio>(5).value == 4);

  package = table1.popAt(9);
  REQUIRE(table1.size() == 1);
  REQUIRE(table1.validate());
  REQUIRE_FALSE(table1.contains(9));

  REQUIRE_THROWS(table2.addWithPackage(9, package));

  table1.clear();
  table2.clear();
  REQUIRE(table1.size() == 0);
  REQUIRE(table1.nTypes() == 3);
  REQUIRE(table2.size() == 0);
  REQUIRE(table2.nTypes() == 4);
}