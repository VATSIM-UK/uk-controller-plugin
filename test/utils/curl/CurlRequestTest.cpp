#include "curl/CurlRequest.h"
#include "http/HttpMethod.h"

using UKControllerPlugin::Curl::CurlRequest;
using UKControllerPluginUtils::Http::HttpMethod;

namespace UKControllerPluginUtilsTest {
    namespace Curl {

        TEST(CurlRequestTest, TestConstructorSetsUri)
        {
            CurlRequest request("http://test.com/abc", CurlRequest::METHOD_GET);
            EXPECT_EQ(0, strcmp(request.GetUri(), "http://test.com/abc"));
        }

        TEST(CurlRequestTest, TestConstructorSetsMethod)
        {
            CurlRequest request("http://test.com/abc", CurlRequest::METHOD_GET);
            EXPECT_EQ(0, strcmp(request.GetMethod(), CurlRequest::METHOD_GET.c_str()));
        }

        TEST(CurlRequestTest, TestConstructorSetsMethodFromHttpMethodEnum)
        {
            CurlRequest request("http://test.com/abc", HttpMethod::Get());
            EXPECT_EQ(0, strcmp(request.GetMethod(), CurlRequest::METHOD_GET.c_str()));
        }

        TEST(CurlRequestTest, TestConstructorSetsMaxRequestTimeIfNotSpecified)
        {
            CurlRequest request("http://test.com/abc", CurlRequest::METHOD_GET);
            EXPECT_EQ(10L, request.GetMaxRequestTime());
        }

        TEST(CurlRequestTest, TestMaxRequestTimeCanBeSet)
        {
            CurlRequest request("http://test.com/abc", CurlRequest::METHOD_GET);
            request.SetMaxRequestTime(155L);
            EXPECT_EQ(155L, request.GetMaxRequestTime());
        }

        TEST(CurlRequestTest, TestConstructorThrowsExceptionIfInvalidMethod)
        {
            EXPECT_THROW(CurlRequest request("http://test.com/abc", "NOTAMETHOD"), std::invalid_argument);
        }

        TEST(CurlRequestTest, GetBodyReturnsBody)
        {
            CurlRequest request("http://test.com/abc", CurlRequest::METHOD_POST);
            request.SetBody("TestBody");
            EXPECT_EQ(0, strcmp(request.GetBody(), "TestBody"));
        }

        TEST(CurlRequestTest, GetBodyThrowsExceptionIfBodyIsSetTwice)
        {
            CurlRequest request("http://test.com/abc", CurlRequest::METHOD_POST);
            EXPECT_NO_THROW(request.SetBody("TestBody"));
            EXPECT_THROW(request.SetBody("TestBody"), std::logic_error);
        }

        TEST(CurlRequestTest, GetBodyThrowsExceptionIfRequestIsGet)
        {
            CurlRequest request("http://test.com/abc", CurlRequest::METHOD_GET);
            EXPECT_THROW(request.SetBody("TestBody"), std::logic_error);
        }

        TEST(CurlRequestTest, GetBodyThrowsExceptionIfRequestIsDelete)
        {
            CurlRequest request("http://test.com/abc", CurlRequest::METHOD_DELETE);
            EXPECT_THROW(request.SetBody("TestBody"), std::logic_error);
        }

        TEST(CurlRequestTest, HeadersCanBeIterated)
        {
            CurlRequest request("http://test.com/abc", CurlRequest::METHOD_GET);
            request.AddHeader("Test1", "Test1v");
            request.AddHeader("Test2", "Test2v");

            CurlRequest::HttpHeaders::const_iterator it = request.cbegin();
            EXPECT_TRUE("Test1" == it->first);
            EXPECT_TRUE("Test1v" == it->second);
            it++;
            EXPECT_TRUE("Test2" == it->first);
            EXPECT_TRUE("Test2v" == it->second);
        }

        TEST(CurlRequestTest, AddHeaderThrowsExceptionIfSameHeaderAddedTwice)
        {
            CurlRequest request("http://test.com/abc", CurlRequest::METHOD_GET);
            EXPECT_NO_THROW(request.AddHeader("Test1", "Test1v"));
            EXPECT_THROW(request.AddHeader("Test1", "Test1v"), std::logic_error);
        }

        TEST(CurlRequestTest, TestHttpVerbs)
        {
            EXPECT_EQ(0, strcmp(CurlRequest::METHOD_GET.c_str(), "GET"));
            EXPECT_EQ(0, strcmp(CurlRequest::METHOD_PUT.c_str(), "PUT"));
            EXPECT_EQ(0, strcmp(CurlRequest::METHOD_POST.c_str(), "POST"));
            EXPECT_EQ(0, strcmp(CurlRequest::METHOD_DELETE.c_str(), "DELETE"));
        }

        TEST(CurlRequestTest, EqualityOperatorReturnsTrueIfEqual)
        {
            CurlRequest request1("http://test.com/abc", CurlRequest::METHOD_PUT);
            request1.SetBody("TestBody");
            request1.AddHeader("Test1", "Test2");
            CurlRequest request2("http://test.com/abc", CurlRequest::METHOD_PUT);
            request2.SetBody("TestBody");
            request2.AddHeader("Test1", "Test2");
            EXPECT_TRUE(request1 == request2);
        }

        TEST(CurlRequestTest, EqualityOperatorReturnsFalseIfUriDifferent)
        {
            CurlRequest request1("http://test.com/abc", CurlRequest::METHOD_PUT);
            request1.SetBody("TestBody");
            request1.AddHeader("Test1", "Test2");
            CurlRequest request2("http://test.com/def", CurlRequest::METHOD_PUT);
            request2.SetBody("TestBody");
            request2.AddHeader("Test1", "Test2");
            EXPECT_FALSE(request1 == request2);
        }

        TEST(CurlRequestTest, EqualityOperatorReturnsFalseIfMethodDifferent)
        {
            CurlRequest request1("http://test.com/abc", CurlRequest::METHOD_PUT);
            request1.SetBody("TestBody");
            request1.AddHeader("Test1", "Test2");
            CurlRequest request2("http://test.com/abc", CurlRequest::METHOD_POST);
            request2.SetBody("TestBody");
            request2.AddHeader("Test1", "Test2");
            EXPECT_FALSE(request1 == request2);
        }

        TEST(CurlRequestTest, EqualityOperatorReturnsFalseIfBodyDifferent)
        {
            CurlRequest request1("http://test.com/abc", CurlRequest::METHOD_PUT);
            request1.SetBody("TestBody");
            request1.AddHeader("Test1", "Test2");
            CurlRequest request2("http://test.com/abc", CurlRequest::METHOD_PUT);
            request2.SetBody("TestBody2");
            request2.AddHeader("Test1", "Test2");
            EXPECT_FALSE(request1 == request2);
        }

        TEST(CurlRequestTest, EqualityOperatorReturnsFalseIfHeadersDifferent)
        {
            CurlRequest request1("http://test.com/abc", CurlRequest::METHOD_PUT);
            request1.SetBody("TestBody");
            request1.AddHeader("Test1", "Test2");
            CurlRequest request2("http://test.com/abc", CurlRequest::METHOD_PUT);
            request2.SetBody("TestBody");
            request2.AddHeader("Test1", "Test4");
            EXPECT_FALSE(request1 == request2);
        }
    } // namespace Curl
} // namespace UKControllerPluginUtilsTest
