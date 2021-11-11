# Stands

## Messages

### Assign a stand to a given aircraft

```JSON
{
  "type": "assign_stand",
  "version": 1,
  "id": "your_message_id",
  "data": {
    "callsign": "RYR123",
    "airfield": "EGSS",
    "stand": "54L"
  }
}
```

The response to this message will be a standard [Success](../Responses/StandardResponses.md#success) or [Failure](../Responses/StandardResponses.md#failure) message, containing your message id.

### Unassign an aircrafts stand

```JSON
{
  "type": "unassign_stand",
  "version": 1,
  "id": "your_message_id",
  "data": {
    "callsign": "RYR123",
  }
}
```

The response to this message will be a standard [Success](../Responses/StandardResponses.md#success) or [Failure](../Responses/StandardResponses.md#failure) message, containing your message id.
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
