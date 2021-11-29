// Copyright 2021 byteihq <kotov038@gmail.com>

#include <BinaryTree.h>
#include <gtest/gtest.h>
#include <string>

TEST(BinaryTree, Construct) {
    {
        BinaryTree<int> b;
        EXPECT_EQ(b.size(), 0);
        EXPECT_TRUE(b.empty());
    }
    {
        BinaryTree<float> b(0.3);
        EXPECT_EQ(b.size(), 1);
        EXPECT_FALSE(b.empty());
        EXPECT_FLOAT_EQ(b.root()->value(), 0.3);
    }
}

class Student {
private:
    std::string name_;
    size_t age_;
public:
    Student() = default;

    explicit Student(const std::string &name, size_t age) : name_(name), age_(age) {}

    ~Student() = default;

    std::string name() const {
        return name_;
    }

    size_t age() const {
        return age_;
    }

    bool operator<(const Student &rhs) const {
        return age_ < rhs.age_;
    }

    bool operator==(const Student &rhs) const {
        return age_ == rhs.age_ && name_ == rhs.name_;
    }
};

TEST(BinaryTree, Append) {
    {
//         -0.2
//    -0.3      4
//            1   5
//          0
        BinaryTree<double> b(-0.2);
        b.append(-0.3);
        b.append(4);
        b.append(1);
        b.append(5);
        b.append(0);
        EXPECT_EQ(b.size(), 6);
        EXPECT_DOUBLE_EQ(b.root()->value(), -0.2);
        EXPECT_DOUBLE_EQ(b.root()->left()->value(), -0.3);
        EXPECT_DOUBLE_EQ(b.root()->right()->value(), 4);
        EXPECT_DOUBLE_EQ(b.root()->right()->right()->value(), 5);
        EXPECT_DOUBLE_EQ(b.root()->right()->left()->value(), 1);
        EXPECT_DOUBLE_EQ(b.root()->right()->left()->left()->value(), 0);
    }
    {
//          b
//        a   c
//             c
        BinaryTree<std::string> b;
        b.append("b");
        b.append("a");
        b.append("c");
        b.append("c");
        EXPECT_EQ(b.size(), 4);
        EXPECT_EQ(b.root()->value(), "b");
        EXPECT_EQ(b.root()->left()->value(), "a");
        EXPECT_EQ(b.root()->right()->value(), "c");
        EXPECT_EQ(b.root()->right()->right()->value(), "c");
    }
    {
//            (Ivanov, 19)
//   (Petrov, 17)       (Smirnov, 22)
//                             (Cplusplusov, 23)
        BinaryTree<Student> b;
        b.append(Student("Ivanov", 19));
        b.append(Student("Smirnov", 22));
        b.append(Student("Petrov", 17));
        b.append(Student("Cplusplusov", 23));
        EXPECT_EQ(b.size(), 4);
        EXPECT_EQ(b.root()->value().name(), "Ivanov");
        EXPECT_EQ(b.root()->value().age(), 19);
        EXPECT_EQ(b.root()->left()->value().name(), "Petrov");
        EXPECT_EQ(b.root()->left()->value().age(), 17);
        EXPECT_EQ(b.root()->right()->value().name(), "Smirnov");
        EXPECT_EQ(b.root()->right()->value().age(), 22);
        EXPECT_EQ(b.root()->right()->right()->value().name(), "Cplusplusov");
        EXPECT_EQ(b.root()->right()->right()->value().age(), 23);
    }
}

TEST(BinaryTree, Has) {
    {
        BinaryTree<char> b;
        EXPECT_FALSE(b.has(1));
    }
    {
        BinaryTree<size_t> b;
        b.append(0);
        EXPECT_TRUE(b.has(0));
        EXPECT_FALSE(b.has(1));
        b.append(1);
        EXPECT_TRUE(b.has(1));
        EXPECT_FALSE(b.has(5));
        b.append(5);
        EXPECT_TRUE(b.has(5));
    }
}

TEST(BinaryTree, Find) {
    {
        BinaryTree<uint16_t> b;
        EXPECT_EQ(b.find(0), nullptr);
    }
    {
//            100
//         20     120
//            30
        BinaryTree<int64_t> b(100);
        EXPECT_EQ(b.find(99), nullptr);
        EXPECT_EQ(b.find(100), b.root());
        b.append(20);
        b.append(30);
        b.append(120);
        EXPECT_EQ(b.find(20), b.root()->left());
        EXPECT_EQ(b.find(30), b.root()->left()->right());
        EXPECT_EQ(b.find(120), b.root()->right());
        EXPECT_EQ(b.find(200), nullptr);
    }
}

TEST(BinaryTree, Remove) {
    {
        BinaryTree<int16_t> b;
        EXPECT_FALSE(b.remove(0));
        EXPECT_TRUE(b.empty());
    }
    {
        BinaryTree<int16_t> b(0);
        EXPECT_TRUE(b.has(0));
        EXPECT_TRUE(b.remove(0));
        EXPECT_FALSE(b.has(0));
        EXPECT_TRUE(b.empty());
    }
    {
//             1
//               2
//                 4
//                3  20
        BinaryTree<int16_t> b;
        b.append(1);
        b.append(2);
        b.append(4);
        b.append(3);
        b.append(20);
        EXPECT_TRUE(b.remove(1));
        EXPECT_FALSE(b.has(1));
        EXPECT_TRUE(b.remove(3));
        EXPECT_FALSE(b.has(3));
        EXPECT_FALSE(b.remove(21));
        EXPECT_EQ(b.size(), 3);
    }
}
