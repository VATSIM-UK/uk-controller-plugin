# Intention Codes

## Events

### Code Updated

```JSON
{
    "type": "intention_code_updated",
    "version": 1,
    "data": {
        "callsign": "BAW123",
        "exit_point": "REDFA",
        "code": "C2",
    }
}
```

In the event that an aircraft will not be exiting the FIR, the `exit_point` shall be set as `null`.
