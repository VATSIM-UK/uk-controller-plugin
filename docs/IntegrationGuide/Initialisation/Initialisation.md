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
    "id": "your_message_id",
    "data": {
        "integration_name": "My Integration",
        "integration_version": "v1.5-beta1",
        "event_subscriptions": [
            {
                "type": "event_one",
                "version": 1
            },
            {
                "type": "event_two",
                "version": 2
            }
        ]
    }
}
```

The integration name and version allows for appropriate logging to take place, in order to help developers on both
sides to debug any issues.

The message id is a string that identifies this message to the plugin, which will be used in order to respond.

## The initialisation response

### Success

After a successful initialisation, the following response will be returned.

```JSON
{
    "type": "initialisation_success",
    "version": 1,
    "id": "your_message_id",
    "data": {
        "ukcp_version": "string"
    }
}
```

Note, that until this response has been sent, the plugin will not action any other messages received from the integration.

**Any messages received before the initialisation response has been sent will be ignored**.

### Failure

After a failed initialisation, the following response will be returned.
```JSON
{
    "type": "initialisation_failure",
    "version": 1,
    "id": "your_message_id",
    "errors": [
        "string"
    ]
}
```
