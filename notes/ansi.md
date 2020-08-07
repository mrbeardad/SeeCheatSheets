<!-- entry begin: ansi color -->
* 8/16/24 colors
    > `${ID}`：见下表
    * `\033[${ID}m`     (普通)
    * `\033[${ID};1m`   (粗体，高亮)
    * `\033[${ID};2m`   (低暗)
<!--  -->
|  color | foreground | background |
|:------:|:----------:|:----------:|
|  black |     30     |     40     |
|   red  |     31     |     41     |
|  green |     32     |     32     |
| yellow |     33     |     33     |
|  blue  |     34     |     34     |
| purple |     35     |     35     |
|  cyan  |     36     |     36     |
|  white |     37     |     37     |

* 256 color
    > `${fg_bg}`：前景为`38m`，背景为`48m`  
    > `${ID}`：见<https://jonasjacek.github.io/colors/>
    * `\033[${fg_bg};5;${ID}m`

* true color
    > `${red|green|blue}`：见系统调色板
    * `\033[${fg_bg};2;${red};${green};${blue}m`
<!-- entry end -->

<!-- entry begin: ansi escape -->
* ansi_escape
<!--  -->
|        escape       |    meaning   |
|:-------------------:|:------------:|
|       `\e[0m`       |     复原     |
|       `\e[1m`       |     加粗     |
|       `\e[2m`       |     低暗     |
|       `\e[3m`       |     斜体     |
|       `\e[4m`       |    下划线    |
|      `\e[4:3m`      |  下划波浪线  |
|       `\e[5m`       |     闪烁     |
|       `\e[7m`       |     反显     |
|       `\e[8m`       |     消隐     |
|       `\e[9m`       |    删除线    |
|      `\e[?25l`      |   隐藏光标   |
<!-- entry end -->

