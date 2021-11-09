from Game import ChangeState
from Game import GetLengthToPlayer
from Game import IsDeath
from Game import Move
from Game import Rotate
from Game import AnimationUpdate
from Game import UpdateTriggerBox
from Game import GetGuardTime
from Game import SetCoolTime


def Update() :

    if(IsDeath()):
         ChangeState(5)
         SetCoolTime(5.0)

    #回転関数呼び出し。
    Rotate()

    #プレイヤーとの距離を調べる。
    lengthToPlayer = GetLengthToPlayer()

    #プレイヤーとの距離が400以上なら待機状態に
    if(lengthToPlayer > 400.0):
        ChangeState(0)

    #ガード時間が0秒になったら攻撃状態にする。
    if(GetGuardTime() == 0.0):
        SetCoolTime(1.2)
        ChangeState(2)

    #アニメーションを更新。
    AnimationUpdate()

    #トリガーボックスを更新。
    UpdateTriggerBox()
