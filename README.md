# stackowey
Stack-based 2D esolang

## Commands
|            ASCII character             |                                       name                                          | description                                                                                                        |
| :------------------------------------: | :---------------------------------------------------------------------------------: | :----------------------------------------------------------------------------------------------------------------- |
|                  `/`                   |                                     "bounce"                                        | Pop two values off the stack. Bounce off if the first one is greater than the second one.                          |
|                  `\`                   |                                   "backbounce"                                      | Pop two values off the stack. Bounce off if the first one is lower than the second one.                            |
|                  `?`                   |                                       "ask"                                         | Get input from the user. Each character is pushed to the stack as its unicode codepoint.                           |
|                  `!`                   |                                       "say"                                         | Output a value to the user. Outputs the UTF-8-encoded character according to its unicode value found on the stack. |
|                  `:`                   |                                     "dubdot"                                        | Push a copy of the topmost value onto the stack                                                                    |
|                  `%`                   |                                    "bouncedot"                                      | Take the top two stack values as coordinates to jump to while not changing direction                               |
| `0`, `1`, `2`, `3`, `4`, `5`, `6`, `7` | "oh", "itch", "nigh", "chan", "fire", "desklamp"/"lamp", "glorp", "wah?"/"wa-ding!" | Push the corresponding octal value to the stack.                                                                   |
|                  `*`                   |                                      "splat"                                        | Pop two stack values, add them and put the result on the stack                                                     |
|                  `_`                   |                                     "downbar"                                       | Pop two stack values, subtract the first one from the second and push the result to the stack                      |
|                  `8`                   |                                     "dubring"                                       | *reserved*                                                                                                         |
|                  `9`                   |                                    "tailring"                                       | Halt                                                                                                               |
|           *all other bytes*            |                                                                                     | No-op                                                                                                              |
