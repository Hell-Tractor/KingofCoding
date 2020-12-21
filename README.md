# King of Coding
这是厦门大学信息学院2020年曾鸣老师C语言大作业

## Installation

- 解压
- 双击KingofCoding.exe运行

## Tutorials

- 点击StartGame按钮
- 选择你想游玩的模式
- 所有模式均加入了暂停功能，前两模式为按下任意键结束暂停且该键会被用作判断
- 任一游戏模式下按下ESC即可打开菜单，再次按下可以关闭
- stagemode（闯关模式）
    - 选择关卡后按下对应按键即可，按错会扣除生命值。
- endlessmode（无尽模式）
    - 在限定时间内输入给定的单词即可加分
    - 不论是按错还是到时间都会扣除生命值
    - 有些字符可以恢复生命值~
- fallingmode（下落模式）
    - 当字符下落到判定线时按下按键即可
    - 按错按键不会产生影响，但是按的过早或过晚都会算作失败哦(MISS)
    - 随着得分增加会提升等级，每次等级提升将恢复生命值并加快字母下落速度
    - 该模式下按下空格即可暂定，再次按下即可开始游戏（开始游戏后需按下空格才会开始下落）
- 比比谁更🐂🍺吧！

## How to add custom stages

- 进入游戏所在文件夹
- 进入stages文件夹
- 将你想要作为stage的文字写在纯文本文档中然后更改后缀名为".stg"（不含引号）
- 程序将自动将".stg"文件合法化并作为关卡使用，你可以在select stage界面找到它

## How to add custom words

- 对于endlessmode
    - 修改keys文件即可，每行一个单词，只能包含拉丁字母
- 对于fallingmode
    - 修改letters文件即可，每行一个单词，只能包含拉丁字母

## Configuration modification

- 打开游戏所在目录下的settings.ini
- 参照修改指定项后的值即可
- <font color=red>请保证修改后的值合法，如不合法将发生不可预期的错误</font>
- <font color=red>请不要修改警告信息后的设置项，随意修改将导致不可预期的错误</font>
- 重启游戏

## Icon modification

- 进入游戏所在目录下的icons文件夹
- 以<font color=red>相同的尺寸</font>替换对应图片（需要保证文件名<font color=red>及后缀</font>均一致）
- 重启游戏

## Future

- 添加设置界面
- 添加高分榜
- 添加存档/读档功能

## Maintainers

mail: chenyiming@stu.xmu.edu.cn

## ChangeLog

[changelog](./changelog.md)