// Copyright Takatoshi Kondo 2018
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "test_main.hpp"
#include "combi_test.hpp"
#include "checker.hpp"

#include <mqtt/optional.hpp>

BOOST_AUTO_TEST_SUITE(test_sub)

BOOST_AUTO_TEST_CASE( pub_qos0_sub_string_single ) {
    auto test = [](boost::asio::io_service& ios, auto& c, auto& s) {
        using packet_id_t = typename std::remove_reference_t<decltype(*c)>::packet_id_t;
        c->set_clean_session(true);


        checker chk = {
            // connect
            cont("h_connack"),
            // subscribe topic1
            cont("h_suback"),
            // unsubscribe topic1
            cont("h_unsuback"),
            // disconnect
            cont("h_close"),
        };

        c->set_connack_handler(
            [&chk, &c]
            (bool sp, std::uint8_t connack_return_code) {
                MQTT_CHK("h_connack");
                BOOST_TEST(sp == false);
                BOOST_TEST(connack_return_code == mqtt::connect_return_code::accepted);
                c->subscribe("topic1", mqtt::qos::at_most_once);
                return true;
            });
        c->set_close_handler(
            [&chk, &s]
            () {
                MQTT_CHK("h_close");
                s.close();
            });
        c->set_error_handler(
            []
            (boost::system::error_code const&) {
                BOOST_CHECK(false);
            });
        c->set_suback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/, std::vector<mqtt::optional<std::uint8_t>> /*results*/) {
                MQTT_CHK("h_suback");
                c->unsubscribe("topic1");
                return true;
            });
        c->set_unsuback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/) {
                MQTT_CHK("h_unsuback");
                c->disconnect();
                return true;
            });
        c->connect();
        ios.run();
        BOOST_TEST(chk.all());
    };
    do_combi_test_sync(test);
}

BOOST_AUTO_TEST_CASE( pub_qos0_sub_string_multi_arg ) {
    auto test = [](boost::asio::io_service& ios, auto& c, auto& s) {
        using packet_id_t = typename std::remove_reference_t<decltype(*c)>::packet_id_t;
        c->set_clean_session(true);


        checker chk = {
            // connect
            cont("h_connack"),
            // subscribe topic1
            cont("h_suback"),
            // unsubscribe topic1
            cont("h_unsuback"),
            // disconnect
            cont("h_close"),
        };

        c->set_connack_handler(
            [&chk, &c]
            (bool sp, std::uint8_t connack_return_code) {
                MQTT_CHK("h_connack");
                BOOST_TEST(sp == false);
                BOOST_TEST(connack_return_code == mqtt::connect_return_code::accepted);
                c->subscribe("topic1", mqtt::qos::at_most_once, "topic2", mqtt::qos::exactly_once);
                return true;
            });
        c->set_close_handler(
            [&chk, &s]
            () {
                MQTT_CHK("h_close");
                s.close();
            });
        c->set_error_handler(
            []
            (boost::system::error_code const&) {
                BOOST_CHECK(false);
            });
        c->set_suback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/, std::vector<mqtt::optional<std::uint8_t>> /*results*/) {
                MQTT_CHK("h_suback");
                c->unsubscribe("topic1", "topic2");
                return true;
            });
        c->set_unsuback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/) {
                MQTT_CHK("h_unsuback");
                c->disconnect();
                return true;
            });
        c->connect();
        ios.run();
        BOOST_TEST(chk.all());
    };
    do_combi_test_sync(test);
}

BOOST_AUTO_TEST_CASE( pub_qos0_sub_string_multi_vec ) {
    auto test = [](boost::asio::io_service& ios, auto& c, auto& s) {
        using packet_id_t = typename std::remove_reference_t<decltype(*c)>::packet_id_t;
        c->set_clean_session(true);


        checker chk = {
            // connect
            cont("h_connack"),
            // subscribe topic1
            cont("h_suback"),
            // unsubscribe topic1
            cont("h_unsuback"),
            // disconnect
            cont("h_close"),
        };

        c->set_connack_handler(
            [&chk, &c]
            (bool sp, std::uint8_t connack_return_code) {
                MQTT_CHK("h_connack");
                BOOST_TEST(sp == false);
                BOOST_TEST(connack_return_code == mqtt::connect_return_code::accepted);
                std::vector<std::tuple<std::string, std::uint8_t>> v;
                v.emplace_back("topic1", mqtt::qos::at_most_once);
                v.emplace_back("topic2", mqtt::qos::exactly_once);
                c->subscribe(v);
                return true;
            });
        c->set_close_handler(
            [&chk, &s]
            () {
                MQTT_CHK("h_close");
                s.close();
            });
        c->set_error_handler(
            []
            (boost::system::error_code const&) {
                BOOST_CHECK(false);
            });
        c->set_suback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/, std::vector<mqtt::optional<std::uint8_t>> /*results*/) {
                MQTT_CHK("h_suback");
                std::vector<std::string> v
                    {
                        "topic1",
                        "topic2",
                    };
                c->unsubscribe(v);
                return true;
            });
        c->set_unsuback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/) {
                MQTT_CHK("h_unsuback");
                c->disconnect();
                return true;
            });
        c->connect();
        ios.run();
        BOOST_TEST(chk.all());
    };
    do_combi_test_sync(test);
}

BOOST_AUTO_TEST_CASE( pub_qos0_sub_string_single_async ) {
    auto test = [](boost::asio::io_service& ios, auto& c, auto& s) {
        using packet_id_t = typename std::remove_reference_t<decltype(*c)>::packet_id_t;
        c->set_clean_session(true);


        checker chk = {
            // connect
            cont("h_connack"),
            // subscribe topic1
            cont("h_suback"),
            // unsubscribe topic1
            cont("h_unsuback"),
            // disconnect
            cont("h_close"),
        };

        c->set_connack_handler(
            [&chk, &c]
            (bool sp, std::uint8_t connack_return_code) {
                MQTT_CHK("h_connack");
                BOOST_TEST(sp == false);
                BOOST_TEST(connack_return_code == mqtt::connect_return_code::accepted);
                c->async_subscribe("topic1", mqtt::qos::at_most_once, [](boost::system::error_code const&) {});
                return true;
            });
        c->set_close_handler(
            [&chk, &s]
            () {
                MQTT_CHK("h_close");
                s.close();
            });
        c->set_error_handler(
            []
            (boost::system::error_code const&) {
                BOOST_CHECK(false);
            });
        c->set_suback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/, std::vector<mqtt::optional<std::uint8_t>> /*results*/) {
                MQTT_CHK("h_suback");
                c->async_unsubscribe("topic1", [](boost::system::error_code const&) {});
                return true;
            });
        c->set_unsuback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/) {
                MQTT_CHK("h_unsuback");
                c->async_disconnect();
                return true;
            });
        c->connect();
        ios.run();
        BOOST_TEST(chk.all());
    };
    do_combi_test_async(test);
}

BOOST_AUTO_TEST_CASE( pub_qos0_sub_string_multi_arg_async ) {
    auto test = [](boost::asio::io_service& ios, auto& c, auto& s) {
        using packet_id_t = typename std::remove_reference_t<decltype(*c)>::packet_id_t;
        c->set_clean_session(true);


        checker chk = {
            // connect
            cont("h_connack"),
            // subscribe topic1
            cont("h_suback"),
            // unsubscribe topic1
            cont("h_unsuback"),
            // disconnect
            cont("h_close"),
        };

        c->set_connack_handler(
            [&chk, &c]
            (bool sp, std::uint8_t connack_return_code) {
                MQTT_CHK("h_connack");
                BOOST_TEST(sp == false);
                BOOST_TEST(connack_return_code == mqtt::connect_return_code::accepted);
                c->async_subscribe(
                    "topic1", mqtt::qos::at_most_once,
                    "topic2", mqtt::qos::exactly_once,
                    [](boost::system::error_code const&) {}
                );
                return true;
            });
        c->set_close_handler(
            [&chk, &s]
            () {
                MQTT_CHK("h_close");
                s.close();
            });
        c->set_error_handler(
            []
            (boost::system::error_code const&) {
                BOOST_CHECK(false);
            });
        c->set_suback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/, std::vector<mqtt::optional<std::uint8_t>> /*results*/) {
                MQTT_CHK("h_suback");
                c->async_unsubscribe(
                    "topic1",
                    "topic2",
                    [](boost::system::error_code const&) {}
                );
                return true;
            });
        c->set_unsuback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/) {
                MQTT_CHK("h_unsuback");
                c->async_disconnect();
                return true;
            });
        c->connect();
        ios.run();
        BOOST_TEST(chk.all());
    };
    do_combi_test_async(test);
}

BOOST_AUTO_TEST_CASE( pub_qos0_sub_string_multi_vec_async ) {
    auto test = [](boost::asio::io_service& ios, auto& c, auto& s) {
        using packet_id_t = typename std::remove_reference_t<decltype(*c)>::packet_id_t;
        c->set_clean_session(true);


        checker chk = {
            // connect
            cont("h_connack"),
            // subscribe topic1
            cont("h_suback"),
            // unsubscribe topic1
            cont("h_unsuback"),
            // disconnect
            cont("h_close"),
        };

        c->set_connack_handler(
            [&chk, &c]
            (bool sp, std::uint8_t connack_return_code) {
                MQTT_CHK("h_connack");
                BOOST_TEST(sp == false);
                BOOST_TEST(connack_return_code == mqtt::connect_return_code::accepted);
                std::vector<std::tuple<std::string, std::uint8_t>> v;
                v.emplace_back("topic1", mqtt::qos::at_most_once);
                v.emplace_back("topic2", mqtt::qos::exactly_once);
                c->async_subscribe(
                    v,
                    [](boost::system::error_code const&) {}
                );
                return true;
            });
        c->set_close_handler(
            [&chk, &s]
            () {
                MQTT_CHK("h_close");
                s.close();
            });
        c->set_error_handler(
            []
            (boost::system::error_code const&) {
                BOOST_CHECK(false);
            });
        c->set_suback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/, std::vector<mqtt::optional<std::uint8_t>> /*results*/) {
                MQTT_CHK("h_suback");
                std::vector<std::string> v
                    {
                        "topic1",
                        "topic2",
                    };
                c->async_unsubscribe(
                    v,
                    [](boost::system::error_code const&) {}
                );
                return true;
            });
        c->set_unsuback_handler(
            [&chk, &c]
            (packet_id_t /*packet_id*/) {
                MQTT_CHK("h_unsuback");
                c->async_disconnect();
                return true;
            });
        c->connect();
        ios.run();
        BOOST_TEST(chk.all());
    };
    do_combi_test_async(test);
}

BOOST_AUTO_TEST_SUITE_END()
