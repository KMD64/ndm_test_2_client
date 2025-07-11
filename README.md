A "calculator" client application that uses Generic Netlink for communication with server part.

For the server part and message descriptions look into https://github.com/KMD64/ndm_test_2_server.

Currently the application generates requests with random field values and sends to server with selected pid, if available

The application uses json11 library for parsing and serialization of JSON objects. https://github.com/dropbox/json11
