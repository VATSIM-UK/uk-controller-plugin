# Initialisation

Once a prospective client has successfully established a connection with the UK Controller Plugin, the next expected
message will be that of initialisation. A client will not be able to send to or receive from the UK Controller Plugin
any data until this has been completed.

## The initialisation message

The initialisation message is structured as follows.

```JSON
{
    "type": "initialise",
    "version": 1,
    "data": {
        "integration_name": "string",
        "integration_version": "string",
        "event_subscriptions": [
            {
                "type": "string",
                "version": "integer"
            },
            {
                "type": "string",
                "version": "integer"
            }
        ]
    }
}
```

The integration name and version allows for appropriate logging to take place, in order to help developers on both
sides to debug any issues.

The message id is a string that identifies this message to the plugin, which will be used in order to respond.

## The initialisation response

After a successful initialisation, the following response will be returned.

```JSON
{
    "type": "initialisation_success",
    "version": 1,
    "data": {
        "ukcp_version": "string"
    }
}
```

Note, that until this response has been sent, the plugin will not action any other messages received from the integration.

**Any messages received before the initialisation response has been sent will be ignored**.
