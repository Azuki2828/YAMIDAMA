from Game import ChangeState
from Game import Rotate
from Game import GetCoolTime
from Game import SetCoolTime
from Game import AnimationUpdate
from Game import UpdateTriggerBox
from Game import JudgeDamage

def Update() :

    #回転関数。
    Rotate()

    #ダメージ判定
    JudgeDamage()

    #クールタイムが0秒になったら攻撃状態に。
    if(GetCoolTime() == 0.0):
        ChangeState(2)

        #クールタイムを1.6秒に設定。
        SetCoolTime(1.6)

    #アニメーションを更新。
    AnimationUpdate()

    #トリガーボックスを更新。
    UpdateTriggerBox()

