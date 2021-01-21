//===- llvm/unittest/IR/TypesTest.cpp - Type unit tests -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "gtest/gtest.h"
using namespace llvm;

namespace {

TEST(TypesTest, StructType) {
  LLVMContext C;

  // PR13522
  StructType *Struct = StructType::create(C, "FooBar");
  EXPECT_EQ("FooBar", Struct->getName());
  Struct->setName(Struct->getName().substr(0, 3));
  EXPECT_EQ("Foo", Struct->getName());
  Struct->setName("");
  EXPECT_TRUE(Struct->getName().empty());
  EXPECT_FALSE(Struct->hasName());
}

TEST(TypesTest, LayoutIdenticalEmptyStructs) {
  LLVMContext C;

  StructType *Foo = StructType::create(C, "Foo");
  StructType *Bar = StructType::create(C, "Bar");
  EXPECT_TRUE(Foo->isLayoutIdentical(Bar));
}

TEST(TypesTest, LayoutIdenticalPackedStructs) {
  LLVMContext C;
  IntegerType *Int32 = IntegerType::get(C, 32);

  StructType *FooPacked = StructType::create({ Int32, Int32 }, "FooPacked", true);
  StructType *FooNotPacked = StructType::create({ Int32, Int32 }, "FooNotPacked", false);
  StructType *BarPacked = StructType::create({ Int32, Int32 }, "BarPacked", true);
  EXPECT_TRUE(FooPacked->isLayoutIdentical(BarPacked));
  EXPECT_FALSE(FooPacked->isLayoutIdentical(FooNotPacked));
  EXPECT_FALSE(BarPacked->isLayoutIdentical(FooNotPacked));
}

TEST(TypesTest, LayoutIdenticalNestedStructs) {
  LLVMContext C;
  IntegerType *Int32 = IntegerType::get(C, 32);

  StructType *Foo = StructType::create({ Int32, Int32 }, "Foo");
  StructType *Bar = StructType::create({ Int32, Int32 }, "Bar");
  EXPECT_TRUE(Foo->isLayoutIdentical(Bar));

  StructType *FooWrapped = StructType::create({ Foo }, "FooWrapped");
  StructType *BarWRapped = StructType::create({ Bar }, "BarWrapped");
  EXPECT_TRUE(FooWrapped->isLayoutIdentical(BarWRapped));
}

TEST(TypesTest, LayoutIdenticalPointers) {
  LLVMContext C;
  PointerType *Ptr1 = PointerType::get(IntegerType::get(C, 8), 0);
  PointerType *Ptr2 = PointerType::get(IntegerType::get(C, 16), 0);

  StructType *Foo = StructType::create({ Ptr1 }, "Foo");
  StructType *Bar = StructType::create({ Ptr2 }, "Bar");
  EXPECT_TRUE(Foo->isLayoutIdentical(Bar));
}

}  // end anonymous namespace
