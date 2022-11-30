# To Do

- [x] Test comms between two boards
- [ ] Example of improved SendBtn debounce
- [ ] Solve World Hunger, tell no one




# Notes of importance

## The SendBtn vs SendBtnWasPressed options that aren't included
Because we don't do anything if those combinations are set

```
// If button is pressed and was previously pressed
} else if ((SendBtn == 1) & (SendBtnWasPressed == 1)) {
      // Don't send

// If button is not pressed and was not previously pressed
} else if ((SendBtn == 0) & (SendBtnWasPressed == 0)) {
      // Nothing to do
```

 
