## Infos

Start value: 50

Ln -> decrement by n
Rn -> increment by n

range: 0 - 99

Result: how many times the position was = 0

## Steps

- Initialize `position` (50)
- Initialize `zero_counter` (0)

- Iterate through lines in `input.txt`
  * Parse the line
    * Extract first letter to determine direction (+ or -)
    * Extract the rest to get the value
    * Calculate `new_position`
      * If `new_position` = 0, increment `zero_counter` by 1
      * set `position` to `new_position`
