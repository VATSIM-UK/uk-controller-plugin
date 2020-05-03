#include "pch/pch.h"
#include "srd/SrdSearchDialog.h"
#include "mock/MockApiInterface.h"

using UKControllerPlugin::Srd::SrdSearchDialog;
using UKControllerPluginTest::Api::MockApiInterface;
using ::testing::Test;
using ::testing::NiceMock;

namespace UKControllerPluginTest {
    namespace Srd {

        class SrdSearchDialogTest : public Test
        {
            public:

                SrdSearchDialogTest() : 
                    dialog(mockApi)
                {
                }

                NiceMock<MockApiInterface> mockApi;
                SrdSearchDialog dialog;
        };

        TEST_F(SrdSearchDialogTest, SearchResultsAreValidWithNoNotes)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
                {"notes", nlohmann::json::array()}
            };
            results.push_back(result1);

            EXPECT_TRUE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreValidWithMinimumLevelNull)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", nlohmann::json()},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
                {"notes", nlohmann::json::array()}
            };
            results.push_back(result1);

            EXPECT_TRUE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreValidWithNotes)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            notesArray.push_back(note1);
            result1["notes"] = notesArray;

            results.push_back(result1);

            EXPECT_TRUE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreValidWithMultipleNotes)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            notesArray.push_back(note1);
            notesArray.push_back(note1);
            result1["notes"] = notesArray;

            results.push_back(result1);

            EXPECT_TRUE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreValidWithMultipleResults)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            notesArray.push_back(note1);
            notesArray.push_back(note1);
            result1["notes"] = notesArray;

            results.push_back(result1);
            results.push_back(result1);

            EXPECT_TRUE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfNotArray)
        {
            EXPECT_FALSE(this->dialog.SearchResultsValid(nlohmann::json::object()));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfNoMinimumLevel)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            result1["notes"] = notesArray;

            nlohmann::json badResult = {
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            badResult["notes"] = notesArray;

            results.push_back(result1);
            results.push_back(badResult);

            EXPECT_FALSE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfMinimumLevelNotvalid)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            result1["notes"] = notesArray;

            nlohmann::json badResult = {
                {"minimum_level", "abc"},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            badResult["notes"] = notesArray;

            results.push_back(result1);
            results.push_back(badResult);

            EXPECT_FALSE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfNoMaximumLevel)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            result1["notes"] = notesArray;

            nlohmann::json badResult = {
                {"minimum_level", 10000},
                {"route_string", "BCN DCT BRI"},
            };
            badResult["notes"] = notesArray;

            results.push_back(result1);
            results.push_back(badResult);

            EXPECT_FALSE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfInvalidMaximumLevel)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            result1["notes"] = notesArray;

            nlohmann::json badResult = {
                {"minimum_level", 10000},
                {"maximum_level", "abc"},
                {"route_string", "BCN DCT BRI"},
            };
            badResult["notes"] = notesArray;

            results.push_back(result1);
            results.push_back(badResult);

            EXPECT_FALSE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfNoRouteString)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            result1["notes"] = notesArray;

            nlohmann::json badResult = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
            };
            badResult["notes"] = notesArray;

            results.push_back(result1);
            results.push_back(badResult);

            EXPECT_FALSE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfInvalidRouteString)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            result1["notes"] = notesArray;

            nlohmann::json badResult = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", 123},
            };
            badResult["notes"] = notesArray;

            results.push_back(result1);
            results.push_back(badResult);

            EXPECT_FALSE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfNoNotes)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            result1["notes"] = notesArray;

            nlohmann::json badResult = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", 123},
            };

            results.push_back(result1);
            results.push_back(badResult);

            EXPECT_FALSE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfNotesNotAnArray)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            result1["notes"] = notesArray;

            nlohmann::json badResult = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", 123},
                {"notes", nlohmann::json::object()}
            };

            results.push_back(result1);
            results.push_back(badResult);

            EXPECT_FALSE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfInvalidNoteId)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            result1["notes"] = notesArray;

            nlohmann::json badResult = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArrayBad = notesArray;
            notesArrayBad.push_back(
                {
                    {"id", "invalid"},
                    {"text", "test"}
                }
            );
            badResult["notes"] = notesArrayBad;

            results.push_back(result1);
            results.push_back(badResult);

            EXPECT_FALSE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, SearchResultsAreInvalidIfInvalidNoteText)
        {
            nlohmann::json results = nlohmann::json::array();
            nlohmann::json result1 = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArray = nlohmann::json::array();
            nlohmann::json note1 = {
                {"id", 1},
                {"text", "Test"}
            };
            result1["notes"] = notesArray;

            nlohmann::json badResult = {
                {"minimum_level", 10000},
                {"maximum_level", 66000},
                {"route_string", "BCN DCT BRI"},
            };
            nlohmann::json notesArrayBad = notesArray;
            notesArrayBad.push_back(
                {
                    {"id", 1},
                    {"text", 1}
                }
            );
            badResult["notes"] = notesArrayBad;

            results.push_back(result1);
            results.push_back(badResult);

            EXPECT_FALSE(this->dialog.SearchResultsValid(results));
        }

        TEST_F(SrdSearchDialogTest, FormatNotesReturnsInvalidNotesIfNotFoundEmptyArray)
        {
            EXPECT_EQ("Notes invalid.", this->dialog.FormatNotes(nlohmann::json::array(), 5));
        }

        TEST_F(SrdSearchDialogTest, FormatNotesReturnsInvalidNotesIfNotFoundIndexOutOfRnage)
        {
            nlohmann::json data = nlohmann::json::array();
            data.push_back(1);
            data.push_back(2);
            data.push_back(3);
            data.push_back(4);
            data.push_back(5);
            EXPECT_EQ("Notes invalid.", this->dialog.FormatNotes(nlohmann::json::array(), 5));
        }

        TEST_F(SrdSearchDialogTest, FormatNotesReturnsNoNotesIfContainsNoNotes)
        {
            nlohmann::json searchResults = nlohmann::json::array();
            searchResults.push_back({
                {"notnotes", "1"}
            });

            EXPECT_EQ("No notes.", this->dialog.FormatNotes(searchResults, 0));
        }

        TEST_F(SrdSearchDialogTest, FormatNotesReturnsNoNotesIfNoNotes)
        {
            nlohmann::json searchResults = nlohmann::json::array();
            searchResults.push_back({
                {"notes", nlohmann::json::array()}
            });

            EXPECT_EQ("No notes.", this->dialog.FormatNotes(searchResults, 0));
        }

        TEST_F(SrdSearchDialogTest, FormatNotesFormatsSingleNote)
        {
            nlohmann::json searchResults = nlohmann::json::array();
            nlohmann::json notes = nlohmann::json::array();
            notes.push_back({
                {"id", 1},
                {"text", "Test"}
            });

            searchResults.push_back({
                {"notes", notes}
            });

            EXPECT_EQ("Note 1\r\n\r\nTest\r\n\r\n", this->dialog.FormatNotes(searchResults, 0));
        }

        TEST_F(SrdSearchDialogTest, FormatNotesFormatsMultipleNotes)
        {
            nlohmann::json searchResults = nlohmann::json::array();
            nlohmann::json notes = nlohmann::json::array();
            notes.push_back({
                {"id", 1},
                {"text", "Test"}
            });
            notes.push_back({
                {"id", 2},
                {"text", "Test 2"}
            });

            searchResults.push_back({
                {"notes", notes}
            });

            EXPECT_EQ(
                "Note 1\r\n\r\nTest\r\n\r\nNote 2\r\n\r\nTest 2\r\n\r\n",
                this->dialog.FormatNotes(searchResults, 0)
            );
        }

        TEST_F(SrdSearchDialogTest, FormatNotesReplacesLineFeedsButNotCarraigeReturns)
        {
            nlohmann::json searchResults = nlohmann::json::array();
            nlohmann::json notes = nlohmann::json::array();
            notes.push_back({
                {"id", 1},
                {"text", "Test 1\nTest 2\r\nTest 3\n\nTest 4"}
                });

            searchResults.push_back({
                {"notes", notes}
            });

            EXPECT_EQ(
                "Note 1\r\n\r\nTest 1\r\nTest 2\r\n\r\nTest 3\r\n\r\nTest 4\r\n\r\n",
                this->dialog.FormatNotes(searchResults, 0)
            );
        }
    }  // namespace Srd
}  // namespace UKControllerPluginTest

