from Game import ChangeState
from Game import Move
from Game import Rotate
from Game import GetCoolTime
from Game import SetCoolTime
from Game import AnimationUpdate
from Game import UpdateTriggerBox
from Game import IsGuarded
from Game import JudgeDamage
from Game import IsDeath

def Update() :

    #クールタイムが0秒になったら咆哮状態に。
    if(GetCoolTime() == 0.0):
        SetCoolTime(1.5)
        ChangeState(6)

    #アニメーションを更新。
    AnimationUpdate()
