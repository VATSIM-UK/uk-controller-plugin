# Overview

The integration protocol consists of JSON messages, passed back and forth over communication channels.

## Message Format

All messages follow the same basic JSON format, containing a message type and a version. Some messages will also require
a unique message identifier, which will allow for a response to be returned.

This is the basic format of a message:

```JSON
{
    "type": "event_type",
    "version": 1,
    "data": {
        "some_field": "some_value"
    }
}
```

## Communication Channels

There is currently one communication channel available for integration.

### Windows Sockets

The UK Controller Plugin listens for connecting integrations via **TCP port 52814** using Windows Sockets. Once a client
has connected, the plugin expects to receive an initialisation message (covered later in this guide) which lets it
know what events the integration wishes to receive.

As TCP can break up any messages into arbitrary sized chunks for the purpose of transmission, it is necessary for each
message to be delimited with a special character so that the boundaries of each message can be determined.

As control characters are not valid in well-formed JSON, the Unit Separator character `\0x1F` is used to delimited
individual JSON messages.
