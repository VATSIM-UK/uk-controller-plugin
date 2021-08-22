# Standard Message Responses

This section provides details of generic responses to messages provided by the UK Controller Plugin.

## Success

This response will be sent following a successful action performed as a result of a message from the client.

```JSON
{
  "type": "action_success",
  "version": 1,
  "id": "your_message_id"
}
```

## Failure

This response will be sent if the plugin fails to perform an action following a message from the client.

```JSON
{
  "type": "action_failure",
  "version": 1,
  "id": "your_message_id",
  "errors": [
    "string",
    "string"
  ]
}
```
