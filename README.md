# LULD Sim

Flags:
| Flag | Takes | Description | Required |
| --- | --- | --- | --- |
| `-open`            | `float` | The openening price for the day | Yes |
| `-luld`            | `bool`  | Whether or not to use LULD (defaults to true) | No |
| `-n_means`         | `int`   | Number of means that will be used (defaults to 1) | No |
| `-order_mean`      | `float` | Mean quantity of orders throughout the day. Can be multiple if `-n_means` is used | Yes |
| `-order_var`       | `float` | Variance in quantity of orders throughou the day. Can be multiple if `-n_means` is used | Yes |
| `-order_weight`    | `float` | Comparative weight of each distribution, if several are used (defaults to 1 for eac) | No |
| `-orders_per_hour` | `int`   | Number of orders scheduled for every hour (defaults to 1000) | No |

