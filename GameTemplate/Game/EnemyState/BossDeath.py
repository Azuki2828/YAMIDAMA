from Game import GetCoolTime
from Game import AnimationUpdate
from Game import GameClear
from Game import UpdateTriggerBox

def Update() :

    #クールタイムが0秒になったら消える。
    if(GetCoolTime() == 0.0):
        #ゲームクリア
        GameClear()

    #アニメーションを更新。
    AnimationUpdate()

    #トリガーボックスを更新。
    UpdateTriggerBox()