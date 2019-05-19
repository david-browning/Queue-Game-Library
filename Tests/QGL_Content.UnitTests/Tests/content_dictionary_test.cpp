#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace qgl::content;

namespace QGL_Content_UnitTests
{
   /*
    Constructor tests test the size and iterator methods.
    */
   TEST_CLASS(ContentDictionaryTests)
   {
      public:
      TEST_METHOD(DefaultConstructor)
      {
         content_dictionary dict;
         Assert::AreEqual(static_cast<size_t>(0),
                          dict.size(),
                          L"The dictionary size should be 0.");

         Assert::IsTrue(dict.begin() == dict.end(),
                        L"The dictionary iterators should be equal.");
      }

      TEST_METHOD(MetadataConstructor)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER meta(
            4,
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
            8);

         content_dictionary dict(meta);

         Assert::AreEqual(static_cast<size_t>(4), dict.size(),
                          L"The dictionary size is not correct");
      }

      TEST_METHOD(CopyConstructor)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER meta(
            4,
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
            8);

         content_dictionary dict(meta);

         content_dictionary dictCopy(dict);

         Assert::AreEqual(dict.size(), dictCopy.size(),
                          L"The dictionary sizes should be the same.");
      }

      TEST_METHOD(MoveConstructor)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER meta(
            4,
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
            8);

         content_dictionary dict(meta);
         content_dictionary dictCopy(dict);
         content_dictionary dictMove(std::move(dict));


         Assert::AreEqual(dictMove.size(), dictCopy.size(),
                          L"The dictionary sizes should be the same.");
      }

      /*
       Also test iterator methods.
       Assume [] operator is correct.
       */
      TEST_METHOD(PushBack)
      {
         content_dictionary dict;
         Assert::AreEqual(static_cast<size_t>(0),
                          dict.size(),
                          L"The dictionary size should be 0.");

         CONTENT_DICTIONARY_ENTRY_BUFFER defaultBuffer;
         dict.push_back(defaultBuffer);
         Assert::AreEqual(static_cast<size_t>(1),
                          dict.size(),
                          L"The dictionary size should be 1.");


         CONTENT_DICTIONARY_ENTRY_BUFFER movedBuffer(defaultBuffer);
         dict.push_back(std::move(movedBuffer));
         Assert::AreEqual(static_cast<size_t>(2),
                          dict.size(),
                          L"The dictionary size should be 2.");

         Assert::IsTrue(dict[0] == defaultBuffer,
                        L"The 0th entry is not correct.");

         Assert::IsTrue(dict[1] == defaultBuffer,
                        L"The 1st entry is not correct.");

         for (auto& e : dict)
         {
            Assert::IsTrue(e == defaultBuffer,
                           L"The iterated entry is not default.");
         }

         CONTENT_DICTIONARY_ENTRY_BUFFER newBuffer(32, 
                                                   CONTENT_METADATA_BUFFER(), 
                                                   64);
         dict.push_back(newBuffer);
         Assert::IsTrue(*(dict.begin() + 2) == newBuffer,
                        L"The last item is not correct.");

      }

      TEST_METHOD(At)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER meta(
            4,
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
            8);
         content_dictionary dict(meta);

         CONTENT_METADATA_BUFFER metadata;
         CONTENT_DICTIONARY_ENTRY_BUFFER entry3(64, metadata, 32);
         dict.at(3) = entry3;

         Assert::IsTrue(dict.at(3) == entry3,
                        L"The entries are not equal.");

         Assert::ExpectException<std::out_of_range>([=](){ dict.at(4); },
                                 L"Expected exception from out of bounds.");
      }

      TEST_METHOD(IndexOperator)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER meta(
            4,
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
            8);
         content_dictionary dict(meta);

         CONTENT_METADATA_BUFFER metadata;
         CONTENT_DICTIONARY_ENTRY_BUFFER entry3(64, metadata, 32);
         dict.at(3) = entry3;

         Assert::IsTrue(dict[3] == entry3,
                        L"The entries are not equal.");

      }

      TEST_METHOD(Buffer)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER meta(
            4,
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
            8);
         content_dictionary dict(meta);

         auto b = dict.buffer();

         Assert::IsTrue(dict.buffer() == meta,
                        L"The metadata should be the same.");
      }

      /*
       Assume copy constructor is correct.
       */
      TEST_METHOD(AssignOperator)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER meta(
            4,
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
            8);

         content_dictionary dict(meta);
         content_dictionary dictCopy = dict;

         Assert::AreEqual(dict.size(), dictCopy.size(),
                          L"The dictionary sizes should be the same.");
      }

      /*
       Assume copy constructor is correct.
       */
      TEST_METHOD(EqualityOperator)
      {
         CONTENT_DICTIONARY_METADATA_BUFFER meta(
            4,
            sizeof(CONTENT_DICTIONARY_ENTRY_BUFFER),
            8);

         content_dictionary dictDefault;
         content_dictionary dict(meta);
         content_dictionary dictCopy(dict);

         Assert::IsTrue(dict == dictCopy,
                        L"The dictionaries should be equal.");

         Assert::IsFalse(dict == dictDefault,
                         L"The dictionaries should not be equal.");

         dictCopy.push_back(CONTENT_DICTIONARY_ENTRY_BUFFER());
         Assert::IsFalse(dict == dictCopy,
                         L"The modified dictionary should not be equal to "
                         L"the original.");
      }
   };
}