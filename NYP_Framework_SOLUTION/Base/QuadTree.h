#ifndef _QUADTREE_H
#define _QUADTREE_H

#include <list>
#include <vector>
#include "Vector3.h"
#include "SingletonTemplate.h"

class EntityBase;

class QTNode
{
private:
	QTNode* m_parent;
	QTNode* m_children[4];

	std::list<EntityBase*> m_entitylist;
	int m_maxentitycount;
	Vector3 m_pos, m_size;
	bool m_NodeSplit;
public:
	QTNode(QTNode* _parent = nullptr, Vector3 _pos = Vector3(), Vector3 _size = Vector3());
	~QTNode();
	std::list<EntityBase*> GetEntityList();
	void SplitNode(QTNode* _parent = nullptr, Vector3 _size = Vector3(), Vector3 _pos = Vector3());
	void AddEntity(EntityBase* _entity);

};

class QuadTree : public Singleton<QuadTree>
{
	friend Singleton<QuadTree>;
private:
	static std::vector<Vector3> m_dir;
	QTNode* m_root;
public:
	QuadTree(Vector3 _size = Vector3(), Vector3 _pos = Vector3());
	~QuadTree();

	void SplitTree(QTNode* _parent = nullptr, Vector3 _size = Vector3(), Vector3 _pos = Vector3());

	void Init(Vector3 _size, Vector3 _pos);
	void Update();
	void Render();
	static Vector3 GetDir(int index);
	void AddEntity(EntityBase* _entity);
};

#endif