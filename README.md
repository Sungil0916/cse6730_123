# LULD Sim

Flags:
| Flag | Takes | Description | Required |
| --- | --- | --- | --- |
| `-open`           | `float` | The openening price for the day | Yes |
| `-luld`           | `bool`  | Whether or not to use LULD (defaults to true) | No |
| `-order_mean`     | `float` | Mean quantity of orders throughout the day | Yes |
| `-order_var`      | `float` | Variance in quantity of orders throughou the day | Yes |
| `orders_per_hour` | `int`   | Number of orders scheduled for every hour (defaults to 1000) | No |
