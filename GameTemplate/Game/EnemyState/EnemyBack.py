from Game import GetCoolTime
from Game import AnimationUpdate
from Game import Delete
from Game import UpdateTriggerBox
from Game import Move
from Game import Rotate
from Game import JudgeDamage
from Game import ChangeState

def Update() :

    #移動関数。
    Move()

    #回転関数。
    Rotate()

    #ダメージ判定。
    JudgeDamage()

    #クールタイムが0秒になったら待機状態に。
    if(GetCoolTime() == 0.0):
        ChangeState(0)

    #アニメーションを更新。
    AnimationUpdate()

    #トリガーボックスを更新。
    UpdateTriggerBox()