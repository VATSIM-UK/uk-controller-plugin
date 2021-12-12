# Missed Approaches

## Events

### Missed Approach Triggered

```JSON
{
  "type": "missed_approach_triggered",
  "version": 1,
  "data": {
    "callsign": "RYR123",
    "triggered_by_user": true,
    "expires_at": "2021-11-11 21:50:00"
  }
}
```

## Messages

### Trigger Missed Approach

```JSON
{
  "type": "trigger_missed_approach",
  "version": 1,
  "id": "your_message_id",
  "data": {
    "callsign": "RYR123"
  }
}
```
