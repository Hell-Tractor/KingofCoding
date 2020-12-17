# King of Coding
这是厦门大学信息学院2020年曾鸣老师C语言大作业

## Installation

- 解压
- 双击KingofCoding.exe运行

## Tutorials

- 点击StartGame按钮
- 选择你想游玩的模式
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

## Notes

- 当前为公测版本(beta 2.0.0)，如发现bug请联系我

## Maintainers

mail: chenyiming@stu.xmu.edu.cn

## ChangeLog

[changelog](./changelog.md)