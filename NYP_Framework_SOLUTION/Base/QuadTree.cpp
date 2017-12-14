#include "QuadTree.h"
#include "EntityBase.h"
#include "CollisionCheckFunctions.h"
#include "Mtx44.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "GL\glew.h"
#include "Collider\Collider.h"
#include "Source\Projectile\Laser.h"
#include "Source/EntityManager.h"


QTNode::QTNode(QTNode * _parent, Vector3 _pos, Vector3 _size) : m_parent(_parent), m_pos(_pos), m_size(_size), m_NodeSplit(false),m_maxentitycount(3),m_depth(0)
{
	for (int i = 0; i < 4; ++i)
	{
		this->m_children[i] = nullptr;
	}
	m_entitylist.clear();
}

QTNode::~QTNode()
{
	this->m_parent = nullptr;
	for (int i = 0; i < 4; ++i)
	{
		this->m_children[i] = nullptr;
	}

	std::list<EntityBase*>::iterator it, end;
	end = m_entitylist.end();
	it = m_entitylist.begin();

	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			delete *it;
			it = m_entitylist.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}
}

std::list<EntityBase*> QTNode::GetEntityList()
{
	return this->m_entitylist;
}

std::list<EntityBase*> QTNode::GetEntityList(Vector3 _pos)
{
    if (m_children[0] == nullptr)
        return m_entitylist;

    for (int i = 0; i < 4; ++i)
    {
        bool c = Collision::CheckOverlap(this->m_children[i]->m_pos - (this->m_children[i]->m_size * 0.5f),
            this->m_children[i]->m_pos + (this->m_children[i]->m_size * 0.5f),
            _pos,
            _pos);
        if (c)
        {
            return m_children[0]->GetEntityList(_pos);
        }
    }
}

void QTNode::SplitNode(QTNode * _parent, Vector3 _size, Vector3 _pos)
{
	Vector3 offset = _size * 0.25f;
	Vector3 childsize = _size * 0.5f;
	childsize.y = _size.y;
	for (int i = 0; i < 4; ++i)
	{
		Vector3 dir = QuadTree::GetDir(i);
		Vector3 childpos;
		childpos.Set(_pos.x + (offset.x * dir.x), _pos.y * dir.y, _pos.z + (offset.z * dir.z));
		m_children[i] = new QTNode(_parent, childpos, childsize);
		m_children[i]->m_parent = this;
		m_children[i]->m_depth = this->m_depth + 1;
	}

	for (auto&_entity : m_entitylist)
	{
		for (int i = 0; i < 4; ++i)
		{
			bool c = Collision::CheckOverlap(this->m_children[i]->m_pos - (this->m_children[i]->m_size * 0.5f),
				this->m_children[i]->m_pos + (this->m_children[i]->m_size * 0.5f),
				_entity->GetPosition() - (_entity->GetScale() * 0.5f),
				_entity->GetPosition() + (_entity->GetScale() * 0.5f));
			if (c)
			{
				this->m_children[i]->AddEntity(_entity);
				break;
			}
		}
	}
}

void QTNode::AddEntity(EntityBase * _entity)
{
	if (this->m_children[0] == NULL)
	{

		this->m_entitylist.push_back(_entity);
	}
	else
	{
		for (int i = 0; i < 4; ++i)
		{
			if (Collision::CheckOverlap(this->m_children[i]->m_pos - (this->m_children[i]->m_size * 0.5f),
				this->m_children[i]->m_pos + (this->m_children[i]->m_size * 0.5f),
				_entity->GetPosition(),
				_entity->GetPosition()))
			{
				this->m_children[i]->AddEntity(_entity);
				break;
			}
		}
	}
	if (this->m_entitylist.size() > m_maxentitycount && this->m_depth < QuadTree::GetInstance()->GetDepthLimit())
	{
		this->SplitNode(this, this->m_size, this->m_pos);
		m_entitylist.clear();
	}
}

void QTNode::Update(double _dt)
{
	for (int i = 0; i < 4; ++i)
	{
		if (m_children[i] == nullptr)
			break;
		this->m_children[i]->Update(_dt);
		
	}
	if (this->GetChildrenEntityNo() < m_maxentitycount && m_children[0] != nullptr)
	{
		this->GetChildrenEntityNo();
		this->MergeNode();
	}

	CheckForCollision();

	std::list<EntityBase*>::iterator it, end;
	end = m_entitylist.end();
	it = m_entitylist.begin();
	while (it != end)
	{
		if ((*it)->IsDone())
		{
			// Delete if done
			EntityManager::GetInstance()->RemoveEntity(*it);
			it = m_entitylist.erase(it);
		}
		else
		{
			// Move on otherwise
			++it;
		}
	}

	if (m_entitylist.size() > 0)
	{
		std::list<EntityBase*> clear;
		for (auto &i : m_entitylist)
		{
			if (Collision::CheckOverlap(this->m_pos - (this->m_size * 0.5f),
				m_pos + (this->m_size * 0.5f),
				i->GetPosition(),
				i->GetPosition()) == false)
			{
				QuadTree::GetInstance()->RefactorThis(i);
				clear.push_back(i);
			}
		}
		for (auto&i : clear)
		{
			m_entitylist.remove(i);
		}
	}

}

void QTNode::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	for (int i = 0; i < 4; ++i)
	{
		if (m_children[i] == nullptr)
			break;
		this->m_children[i]->Render();
	}


	for (auto &i : this->m_entitylist)
	{
		i->Render();
		modelStack.PushMatrix();
		RenderHelper::DrawLine(Vector3(this->m_pos.x, m_pos.y - 5, m_pos.z), i->GetPosition());
		modelStack.PopMatrix();
	}
	modelStack.PushMatrix();
	modelStack.Translate(m_pos.x, m_pos.y - 5, m_pos.z);
	RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("sphere"));

	modelStack.PopMatrix();
	for (int i = 0; i < 4; ++i)
	{
		modelStack.PushMatrix();
		Vector3 dir = QuadTree::GetDir(i);
		Vector3 pos1(m_pos.x + m_size.x * 0.5 * dir.x, -5, m_pos.z + m_size.z * 0.5 * dir.z);
		Vector3 pos2;
		if (i + 1 >= 4)
		{
			dir = QuadTree::GetDir(0);
		}
		else
		{
			dir = QuadTree::GetDir(i + 1);
		}
		pos2.Set(m_pos.x + m_size.x * 0.5 * dir.x, -5, m_pos.z + m_size.z * 0.5 * dir.z);
		RenderHelper::DrawLine(pos1, pos2);
		modelStack.PopMatrix();
	}
}

void QTNode::CheckForCollision(void)
{
	// Check for Collision
	std::list<EntityBase*>::iterator colliderThis, colliderThisEnd;
	std::list<EntityBase*>::iterator colliderThat, colliderThatEnd;

	colliderThisEnd = m_entitylist.end();
	for (colliderThis = m_entitylist.begin(); colliderThis != colliderThisEnd; ++colliderThis)
	{
		// Check if this entity is a CLaser type
		if ((*colliderThis)->GetIsLaser())
		{
			// Dynamic cast it to a CLaser class
			CLaser* thisEntity = dynamic_cast<CLaser*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = m_entitylist.end();
			int counter = 0;
			for (colliderThat = m_entitylist.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					Vector3 hitPosition = Vector3(0, 0, 0);

					// Get the minAABB and maxAABB for (*colliderThat)
					CCollider *thatCollider = dynamic_cast<CCollider*>(*colliderThat);
					Vector3 thatMinAABB = (*colliderThat)->GetPosition() + thatCollider->GetMinAABB();
					Vector3 thatMaxAABB = (*colliderThat)->GetPosition() + thatCollider->GetMaxAABB();

					if (Collision::CheckLineSegmentPlane(thisEntity->GetPosition(),
						thisEntity->GetPosition() - thisEntity->GetDirection() * thisEntity->GetLength(),
						thatMinAABB, thatMaxAABB,
						hitPosition) == true)
					{
						(*colliderThis)->SetIsDone(true);
						(*colliderThat)->SetIsDone(true);
					}
				}
			}
		}
		else if ((*colliderThis)->HasCollider())
		{
			// This object was derived from a CCollider class, then it will have Collision Detection methods
			//CCollider *thisCollider = dynamic_cast<CCollider*>(*colliderThis);
			EntityBase *thisEntity = dynamic_cast<EntityBase*>(*colliderThis);

			// Check for collision with another collider class
			colliderThatEnd = m_entitylist.end();
			int counter = 0;
			for (colliderThat = m_entitylist.begin(); colliderThat != colliderThatEnd; ++colliderThat)
			{
				if (colliderThat == colliderThis)
					continue;

				if ((*colliderThat)->HasCollider())
				{
					EntityBase *thatEntity = dynamic_cast<EntityBase*>(*colliderThat);
					if (Collision::CheckSphereCollision(thisEntity, thatEntity))
					{
						if (Collision::CheckAABBCollision(thisEntity, thatEntity))
						{
							thisEntity->SetIsDone(true);
							thatEntity->SetIsDone(true);
						}
					}
				}
			}
		}
	}
	
	return;
}

void QTNode::PrintNode(int RootNo,int depth)
{
	for (int i = 0; i < depth; ++i)
	{
		std::cout << "	";
	}
	std::cout << "-" << "Node " << RootNo <<"("<< m_entitylist.size()<<")"<<std::endl;

	for (int i = 0; i < 4; ++i)
	{
		if (m_children[i] == nullptr)
			break;
		this->m_children[i]->PrintNode(i,depth + 1);
	}
}

int QTNode::GetChildrenEntityNo()
{
	if (m_children[0] == nullptr)
		return this->m_entitylist.size();
	int TotalSize = 0;
	for (int i = 0; i < 4; ++i)
		TotalSize += m_children[i]->GetChildrenEntityNo();
	return TotalSize;
}

void QTNode::MergeNode()
{
	if (!m_children[0])
		return;
	for (int i = 0; i < 4; ++i)
	{
		if (m_children[i]->m_children[0] != nullptr)
			m_children[i]->MergeNode();
		for (auto &i : m_children[i]->m_entitylist)
		{
			this->m_entitylist.push_back(i);
		}
		delete m_children[i];
		m_children[i] = nullptr;
	}
	std::cout << "FINAL: " << m_entitylist.size() << std::endl;
}

std::vector<Vector3> QuadTree::m_dir;

QuadTree::QuadTree(Vector3 _size, Vector3 _pos)
{
	m_dir.push_back(Vector3(1, 1, 1));
	m_dir.push_back(Vector3(-1, 1, 1));
    m_dir.push_back(Vector3(-1, 1, -1));
	m_dir.push_back(Vector3(1, 1, -1));
	m_depth_limit = 4;
}

QuadTree::~QuadTree()
{

}

void QuadTree::Init(Vector3 _size, Vector3 _pos)
{
	m_root = new QTNode(nullptr, _pos, _size);
}

void QuadTree::Update(double _dt)
{
	m_root->Update(_dt);
	for (auto &i : m_refactorentitylist)
	{
		AddEntity(i);
	}
	m_refactorentitylist.clear();
}

void QuadTree::Render()
{
	m_root->Render();
}

Vector3 QuadTree::GetDir(int index)
{
	if (index > m_dir.size())
		return Vector3();
	return m_dir[index];
}

void QuadTree::AddEntity(EntityBase * _entity)
{
	m_root->AddEntity(_entity);
}

void QuadTree::PrintTree()
{
	std::cout << "START" << std::endl;

	this->m_root->PrintNode(0,0);
	std::cout << "END" << std::endl;

}

void QuadTree::RefactorThis(EntityBase * _entity)
{
	m_refactorentitylist.push_back(_entity);
	std::cout << "Refactor" << std::endl;
}

std::list<EntityBase*> QuadTree::GetEntityList(Vector3 _pos)
{
    return m_root->GetEntityList(_pos);
}
