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

    #死亡判定
    if(IsDeath()):
         ChangeState(5)
         SetCoolTime(5.0)

    #移動関数。
    Move()

    #ダメージ判定
    JudgeDamage()

    #クールタイムが0秒になったら待機状態に。
    if(GetCoolTime() == 0.0):
        ChangeState(1)

    #アニメーションを更新。
    AnimationUpdate()

    #トリガーボックスを更新。
    UpdateTriggerBox()
