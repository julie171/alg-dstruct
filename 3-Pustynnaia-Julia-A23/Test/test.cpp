#include "pch.h"
#include "LabA.h"

TEST(ReadWord_Test, ReadWord_ReadEmptyFile_returnNegative2) {
	FILE* f;
	fopen_s(&f, "EmptyFile.txt", "r");
	word_t* word = ReadWord(f);
	EXPECT_TRUE(word->length == -2);
	free(word->word);
	free(word);
	fclose(f);
}
TEST(ReadWord_Test, ReadWord_Read1Word_returnValidVal) {
	FILE* f;
	fopen_s(&f, "Word.txt", "r");
	word_t* word = ReadWord(f);
	EXPECT_STREQ(word->word, "access");
	EXPECT_TRUE(word->length == -1);
	free(word->word);
	free(word);
	fclose(f);
}
TEST(ReadWord_Test, ReadWord_ReadSeparator_return0) {
	FILE* f;
	fopen_s(&f, "Separator.txt", "r");
	word_t* word = ReadWord(f);
	EXPECT_TRUE(word->length == 0);
	free(word->word);
	free(word);
	fclose(f);
}
TEST(ReadWord_Test, ReadWord_ReadWordAndSeparator_returnValidVal) {
	FILE* f;
	fopen_s(&f, "WordAndSeparator.txt", "r");
	word_t* word = ReadWord(f);
	EXPECT_TRUE(word->length > 0);
	EXPECT_STREQ(word->word, "access");
	free(word->word);
	free(word);
	fclose(f);
}
TEST(ReadWord_Test, ReadWord_ReadSeparators_return0) {
	FILE* f;
	fopen_s(&f, "Separators.txt", "r");
	word_t* word = ReadWord(f);
	EXPECT_TRUE(word->length == 0);
	EXPECT_STREQ(word->word, "\0");
	free(word->word);
	free(word);
	fclose(f);
}
TEST(ChangeRegister_Test, ChangeRegister_CapitalLetter_returnLowercaseLetter) {
	char letter = 'A';
	letter = ChangeRegister(letter);
	EXPECT_EQ(letter, 'a');
}
TEST(ChangeRegister_Test, ChangeRegister_LowercaseLetter_returnLowercaseLetter) {
	char letter = 'a';
	letter = ChangeRegister(letter);
	EXPECT_EQ(letter, 'a');
}
TEST(Alphabet_Test, Alphabet_2SimilarWords_return3) {
	char word1[6] = "pique";
	char word2[6] = "piano";
	int letter = Alphabet(word1, word2, 5, 1);
	EXPECT_TRUE(letter == 2);
}
TEST(Alphabet_Test, Alphabet_2IdenticalWords_returnNegative1) {
	char word1[4] = "pie";
	char word2[4] = "pie";
	int letter = Alphabet(word1, word2, 3, 0);
	EXPECT_TRUE(letter == -1);
}
TEST(Alphabet_Test, Alphabet_2DifferentWords_return0) {
	char word1[5] = "find";
	char word2[5] = "cake";
	int letter = Alphabet(word1, word2, 4, 0);
	EXPECT_TRUE(letter == 0);
}
TEST(CreateList_Test, CreateList_Words_returnValidItemsInValidOrder) {
	FILE* f;
	word_t* word;
	word_t* head = (word_t*)malloc(sizeof(word_t));
	fopen_s(&f, "Words.txt", "r");
	head = CreateList(f);
	word = head->next;
	EXPECT_STREQ(word->word, "a");
	word = word->next;
	EXPECT_STREQ(word->word, "I");
	word = word->next;
	EXPECT_STREQ(word->word, "age");
	word = word->next;
	EXPECT_STREQ(word->word, "was");
	word = word->next;
	EXPECT_STREQ(word->word, "from");
	word = word->next;
	EXPECT_STREQ(word->word, "young");
	word = word->next;
	EXPECT_STREQ(word->word, "broken");
	Clearing(head);
	fclose(f);
}
TEST(CreateList_Test, CreateList_EmptyFile_return0) {
	FILE* f;
	word_t* head = (word_t*)malloc(sizeof(word_t));
	fopen_s(&f, "EmptyFile.txt", "r");
	head = CreateList(f);
	EXPECT_TRUE(head->next->next == NULL);
	Clearing(head);
	fclose(f);
}
TEST(CreateList_Test, CreateList_SortedWords_returnValidItemsInValidOrder) {
	FILE* f;
	word_t* word;
	word_t* head = (word_t*)malloc(sizeof(word_t));
	fopen_s(&f, "SortedWords.txt", "r");
	head = CreateList(f);
	word = head->next;
	EXPECT_STREQ(word->word, "Can");
	word = word->next;
	EXPECT_STREQ(word->word, "see");
	word = word->next;
	EXPECT_STREQ(word->word, "the");
	word = word->next;
	EXPECT_STREQ(word->word, "the");
	word = word->next;
	EXPECT_STREQ(word->word, "yOu");
	word = word->next;
	EXPECT_STREQ(word->word, "dark");
	word = word->next;
	EXPECT_STREQ(word->word, "dark");
	word = word->next;
	EXPECT_STREQ(word->word, "darY");
	Clearing(head);
	fclose(f);
}
TEST(CreateList_Test, CreateList_1Word_returnValidItem) {
	FILE* f;
	word_t* head = (word_t*)malloc(sizeof(word_t));
	fopen_s(&f, "1Word.txt", "r");
	head = CreateList(f);
	EXPECT_STREQ(head->next->word, "fish");
	Clearing(head);
	fclose(f);
}