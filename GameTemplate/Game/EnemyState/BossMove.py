from Game import Move
from Game import Rotate
from Game import ChangeState
from Game import GetLengthToPlayer
from Game import SetCoolTime
from Game import AnimationUpdate
from Game import UpdateTriggerBox
from Game import JudgeDamage

def Update():

    #移動関数。
    Move()

    #回転関数。
    Rotate()

    #ダメージ判定
    JudgeDamage()

    #プレイヤーとの距離を調べる。
    lengthToPlayer = GetLengthToPlayer()


    #プレイヤーとの距離が120以下なら攻撃態勢に入る。
    if(lengthToPlayer <= 120.0):
        ChangeState(3)

        #クールタイムを4.0秒に設定。
        SetCoolTime(3.0)
    #プレイヤーとの距離が500より遠いならジャンプ攻撃状態に。
    elif(lengthToPlayer > 500.0):
        ChangeState(2)
        SetCoolTime(3.6)



    #アニメーションを更新。
    AnimationUpdate()

    #トリガーボックスを更新。
    UpdateTriggerBox()
