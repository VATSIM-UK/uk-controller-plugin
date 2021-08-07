# Intention Codes

## Messages

### Assign a stand to a given aircraft

```JSON
{
  "type": "assign_stand",
  "version": 1,
  "data": {
    "message_id": "your_message_id",
    "callsign": "RYR123",
    "airfield": "EGSS",
    "stand": "54L"
  }
}
```

The response to this message will be a standard [Success message](../Responses/StandardResponses.md#success), containing your message id.

## Events

### Stand Assigned

```JSON
{
  "type": "stand_assigned",
  "version": 1,
  "data": {
    "callsign": "RYR123",
    "airfield": "EGSS",
    "stand": "54L"
  }
}
```

### Stand Unassigned

```JSON
{
  "type": "stand_unassigned",
  "version": 1,
  "data": {
    "callsign": "BAW123"
  }
}
```
