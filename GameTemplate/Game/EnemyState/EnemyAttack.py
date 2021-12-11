from Game import ChangeState
from Game import Move
from Game import Rotate
from Game import GetCoolTime
from Game import SetCoolTime
from Game import AnimationUpdate
from Game import UpdateTriggerBox
from Game import IsGuarded
from Game import JudgeDamage

def Update() :

    #ガードされたら弾き状態に
    if(IsGuarded()):
        ChangeState(5)
        SetCoolTime(0.4)

    #移動関数。
    Move()

    #回転関数。
    Rotate()

    #ダメージ判定
    JudgeDamage()

    #クールタイムが0秒になったら待機状態に。
    if(GetCoolTime() == 0.0):
        ChangeState(0)

    #アニメーションを更新。
    AnimationUpdate()

    #トリガーボックスを更新。
    UpdateTriggerBox()
