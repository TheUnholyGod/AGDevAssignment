#include "QuadTree.h"
#include "EntityBase.h"
#include "CollisionCheckFunctions.h"
#include "Mtx44.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "GL\glew.h"


QTNode::QTNode(QTNode * _parent, Vector3 _pos, Vector3 _size) : m_parent(_parent), m_pos(_pos), m_size(_size), m_NodeSplit(false),m_maxentitycount(3)
{
	for (int i = 0; i < 4; ++i)
	{
		this->m_children[i] = nullptr;
	}
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

void QTNode::SplitNode(QTNode * _parent, Vector3 _size, Vector3 _pos)
{
	std::cout << "Split" << std::endl;
	Vector3 offset = _size * 0.25f;
	Vector3 childsize = _size * 0.5f;
	for (int i = 0; i < 4; ++i)
	{
		Vector3 dir = QuadTree::GetDir(i);
		Vector3 childpos;
		childpos.Set(_pos.x + (offset.x * dir.x), _pos.y * dir.y, _pos.z + (offset.z * dir.z));
		m_children[i] = new QTNode(_parent, childpos, childsize);
		m_children[i]->m_parent = this;
	}

	for (auto&_entity : m_entitylist)
	{
		for (int i = 0; i < 4; ++i)
		{
			bool c = Collision::CheckOverlap(this->m_children[i]->m_pos - (this->m_children[i]->m_size * 0.5f),
				this->m_children[i]->m_pos + (this->m_children[i]->m_size * 0.5f),
				_entity->GetPosition() - (_entity->GetScale() * 0.5f),
				_entity->GetPosition() + (_entity->GetScale() * 0.5f));
			std::cout << c << std::endl;
			if (c)
			{
				this->m_children[i]->AddEntity(_entity);
				std::cout << "Added to" << i << std::endl;
				break;
			}
		}
	}
}

void QTNode::AddEntity(EntityBase * _entity)
{
	if (this->m_children[0] == NULL)
	{
		std::cout << "Added"<< std::endl;

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
				std::cout << "Added to" << i << std::endl;
				std::cout << m_entitylist.size() << std::endl;
				break;
			}
		}
	}
	if (this->m_entitylist.size() > m_maxentitycount)
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
	for (auto &i : this->m_entitylist)
		i->Update(_dt);

}

void QTNode::Render()
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	for (int i = 0; i < 4; ++i)
	{
        if (m_children[i] == nullptr)
        {
            //MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
            //modelStack.PushMatrix();
            //modelStack.Translate(m_pos.x, m_pos.y - 4.9f, m_pos.z);
            //modelStack.Scale(m_size.x, 1 , m_size.z);

            //modelStack.Rotate(-90, 1, 0, 0);
            //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            //RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("GRIDMESH"));
            //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            //RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("sphere"));

            //modelStack.PopMatrix();
            break;
        }
		this->m_children[i]->Render();
      /*  Vector3 pos1 = m_children[i]->m_pos;
        Vector3 pos2;
        if (i + 1 >= 4)
        {
            pos2 = m_children[0]->m_pos;
            modelStack.PushMatrix();
            RenderHelper::DrawLine(pos2, pos1);
            modelStack.PopMatrix();
        }
        else
        {
            pos2 = m_children[i + 1]->m_pos;
            modelStack.PushMatrix();
            RenderHelper::DrawLine(pos1, pos2);
            modelStack.PopMatrix();
        }*/


	}
	for (auto &i : this->m_entitylist)
		i->Render();
    modelStack.PushMatrix();
    modelStack.Translate(m_pos.x, m_pos.y - 4.9f, m_pos.z);
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

std::vector<Vector3> QuadTree::m_dir;

QuadTree::QuadTree(Vector3 _size, Vector3 _pos)
{
	m_dir.push_back(Vector3(1, 1, 1));
	m_dir.push_back(Vector3(-1, 1, 1));
    m_dir.push_back(Vector3(-1, 1, -1));
	m_dir.push_back(Vector3(1, 1, -1));
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
