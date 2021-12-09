#pragma once

namespace nsMyGame {

	//�U���p�̓����蔻��N���X
	class CAttackCollision : public CIGameObject
	{
	public:
		/**
		 * @brief �f�X�g���N�^�B�R���W�������폜����B
		*/
		~CAttackCollision();

		/**
		 * @brief �{�b�N�X�̓����蔻����쐬����֐��B
		*/
		void CreateBox(const CVector3& pos, const CQuaternion& rot, const CVector3& size);

		/**
		 * @brief ���[���h�s���ݒ肷��֐��B
		 * @param worldMatrix ���[���h�s��
		*/
		void SetWorldMatrix(const CMatrix& matrix) {

			CVector3 position;
			position.x = matrix.m[3][0];
			position.y = matrix.m[3][1];
			position.z = matrix.m[3][2];
			SetPosition(position);

			CQuaternion rotation;
			rotation.SetRotation(matrix);
			SetRotation(rotation);
		}

		/**
		 * @brief �L���^�C����ݒ肷��֐��B
		 * @param activeTime �L���^�C��
		*/
		void SetActiveTime(const float activeTime){

			m_activeTime = activeTime;
		}

		/**
		 * @brief �Փ˂����R���W���������g���H
		 * @param contactObject �����蔻��I�u�W�F�N�g
		 * @return �R���W���������g���ǂ����̃t���O
		*/
		bool IsSelf(const btCollisionObject& contactObject) {

			return m_ghostBox.IsSelf(contactObject);
		}
	private:
		/**
		 * @brief Update()�֐��̑O�Ɉ�x�����Ă΂��֐��B
		 * @return ���������H
		*/
		bool Start()override final;

		/**
		 * @brief �X�V�֐��B
		*/
		void Update()override final;

		/**
		 * @brief ���W��ݒ肷��֐��B
		 * @param pos ���W
		*/
		void SetPosition(const CVector3& pos) {

			m_ghostBox.SetPosition(pos);
		}

		/**
		 * @brief ��]��ݒ肷��֐��B
		 * @param rot ��]
		*/
		void SetRotation(const CQuaternion& rot) {

			m_ghostBox.SetRotation(rot);
		}
	private:
		float m_activeTime = 0.0f;						//�L���^�C��
		CPhysicsGhostObject m_ghostBox;					//�g���K�[�{�b�N�X
	};
}
