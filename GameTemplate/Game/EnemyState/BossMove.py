from Game import Move
from Game import Rotate
from Game import ChangeState
from Game import GetLengthToPlayer
from Game import SetCoolTime
from Game import AnimationUpdate
from Game import UpdateTriggerBox
from Game import JudgeDamage
from Game import NoticePlayer
from Game import IsDeath
import random
import time

def Update():

    #死亡判定
    if(IsDeath()):
         ChangeState(5)
         SetCoolTime(5.0)

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

        #乱数を生成
        carentTime = time.time()
        random.seed(carentTime)
        attack = random.randrange(2)
        
        if(attack == 0):

            ChangeState(3)
            SetCoolTime(2.0)
        elif(attack == 1):
        
            ChangeState(2)
            SetCoolTime(3.6)
        
    #プレイヤーとの距離が500より遠いならジャンプ攻撃状態に。
    elif(lengthToPlayer > 800.0):
        ChangeState(2)
        SetCoolTime(3.6)

    #アニメーションを更新。
    AnimationUpdate()

    #トリガーボックスを更新。
    UpdateTriggerBox()
