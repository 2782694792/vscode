#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

// 节点
class Node
{
public:
    /// @brief 数据
    char m_cData;
    /// @brief 是否访问
    bool m_bVisited;
    

public:
    /// @brief 节点：数据+可访问
    /// @param data 节点值
    Node(char data = 0)
    {
        m_cData = data;
        m_bVisited = false;
    }

    /// @brief 当前节点数据与访问控制
    /// @param node
    /// @return 返回详细的节点
    Node operator=(const Node &node)
    {
        if (this == &node)
        {
            return *this;
        }
        this->m_cData = node.m_cData;
        this->m_bVisited = node.m_bVisited;
        return *this;
    }

    /// @brief 指向当前节点
    /// @param node 当前目标节点
    Node(const Node &node)
    {
        if (this == &node)
        {
            return;
        }
        *this = node;
    }
};

// 边
class Edge
{
public:
    // 头顶点
    int m_iNodeIndexA;
    // 尾顶点
    int m_iNodeIndexB;
    // 权重
    int m_iWeightValue;
    // 是否被选中
    bool m_bSelected;

public:
    Edge(int nodeIndexA = 0, int nodeIndexB = 0, int weightValue = 0) : m_iNodeIndexA(nodeIndexA),
                                                                        m_iNodeIndexB(nodeIndexB),
                                                                        m_iWeightValue(weightValue),
                                                                        m_bSelected(false){};

    Edge(const Edge &edge)
    {
        if (this == &edge)
        {
            return;
        }
        *this = edge;
    };

    Edge &operator=(const Edge &edge)
    {
        if (this == &edge)
        {
            return *this;
        }
        this->m_iNodeIndexA = edge.m_iNodeIndexA;
        this->m_iNodeIndexB = edge.m_iNodeIndexB;
        this->m_iWeightValue = edge.m_iWeightValue;
        this->m_bSelected = edge.m_bSelected;
        return *this;
    }
};

// 图
class CMap
{
private:
    // 顶点总数
    int m_iCapacity;
    // 当前顶点数量
    int m_iNodeCount;
    // 顶点集合
    Node *m_pNodeArray;
    // 邻接矩阵
    int *m_pMatrix;
    // 最小生成树边集合
    Edge *m_pEdgeArray;

public:
    /// @brief 构造邻接矩阵
    /// @param iCapacity 顶点总数
    CMap(int iCapacity)
    {
        m_iCapacity = iCapacity;
        m_iNodeCount = 0;
        m_pNodeArray = new Node[m_iCapacity];
        m_pMatrix = new int[m_iCapacity * m_iCapacity];
        memset(m_pMatrix, 0, m_iCapacity * m_iCapacity * sizeof(int));
        m_pEdgeArray = new Edge[m_iCapacity - 1];
    }
    ~CMap()
    {
        delete[] m_pNodeArray;
        delete[] m_pMatrix;
        // delete[] m_pEdgeArray;
    }

public:
    /// @brief 广度优先遍历
    /// @param index
    void breadthFirstTraverse(int index)
    {
        // 输出首个访问点
        cout << m_pNodeArray[index].m_cData << "  ";
        m_pNodeArray[index].m_bVisited = true;

        vector<int> curVec;
        curVec.push_back(index);

        breadthFirstTraverseImpl(curVec);
    }

    /// @brief 深度优先遍历
    /// 以当前节点进行深度遍历，从连接的第一个节点开始进行深度遍历
    /// @param index 当前索引节点
    void depthFirstTraverse(int index)
    {
        // 当前访问
        cout << m_pNodeArray[index].m_cData << "  ";

        m_pNodeArray[index].m_bVisited = true;

        int val = 0;
        // 节点遍历
        for (int i = 0; i < m_iCapacity; i++)
        {
            // 获取权重
            getValueFromMatrix(index, i, val);
            if (0 != val)
            {
                // 绕开已经访问
                if (m_pNodeArray[i].m_bVisited)
                {
                    continue;
                }
                // 递归深度
                depthFirstTraverse(i);
            }
            else
            {
                continue;
            }
        }
    }

private:
    /// @brief 广度优先遍历实现
    /// 遍历完当前节点下的所有连接点之后进行下个连接点的广度遍历
    /// @param preVec
    void breadthFirstTraverseImpl(vector<int> preVec)
    {
        int val = 0;
        vector<int> curVec;
        // 1
        for (int i = 0; i < (int)preVec.size(); i++)
        {
            // 6
            for (int j = 0; j < m_iCapacity; j++)
            {
                // 获取权重 : 行、列、权重值
                getValueFromMatrix(preVec[i], j, val);
                if (0 != val)
                {
                    // 遍历未访问过的节点
                    // fixme : 访问设置：访问过、权重值、不超过最后一个节点
                    if (m_pNodeArray[i].m_bVisited)
                    {
                        continue;
                    }
                    // 遍历数据
                    cout << m_pNodeArray[i].m_cData << "  ";
                    // 节点确定访问
                    m_pNodeArray[i].m_bVisited = true;
                    // 存入当前容器，进行判断，
                    curVec.push_back(j);
                }
                else
                {
                    continue;
                }
            }
        }

        if (curVec.empty())
        {
            return;
        }
        else
        {
            breadthFirstTraverseImpl(curVec);
        }
    }

    /// @brief 取最小边
    /// @param edgeVec 待选边集合
    /// @return 最小边索引
    int getMinEdge(const vector<Edge> &edgeVec)
    {
        int min = 0, minEdge = -1;
        for (int i = 0; i < (int)edgeVec.size(); i++)
        {
            // 被选中
            if (edgeVec[i].m_bSelected)
            {
                continue;
            }
            // 设置存在的权重和边
            min = edgeVec[i].m_iWeightValue;
            minEdge = i;
        }

        if (min == 0 && minEdge == -1)
        {
            return -1;
        }

        for (int i = 0; i < (int)edgeVec.size(); i++)
        {
            if (edgeVec[i].m_bSelected)
            {
                continue;
            }
            if (min > edgeVec[i].m_iWeightValue)
            {
                min = edgeVec[i].m_iWeightValue;
                minEdge = i;
            }
        }

        return minEdge;
    }

    /// @brief  查询是否存在目标节点
    bool isInSet(const vector<int> &nodeSet, int target)
    {
        for (int i = 0; i < (int)nodeSet.size(); i++)
        {
            if (nodeSet[i] == target)
                return true;
        }

        return false;
    }

    /// @brief 合并节点容器
    void mergeNodeSet(vector<int> &nodeSetA, const vector<int> &nodeSetB)
    {
        for (size_t i = 0; i < (int)nodeSetB.size(); i++)
        {
            nodeSetA.push_back(nodeSetB[i]);
        }
    }

public:
    /// @brief 打印矩阵
    void printMatrix()
    {
        for (int i = 0; i < m_iCapacity; i++)
        {
            for (int j = 0; j < m_iCapacity; j++)
            {
                cout << m_pMatrix[i * m_iCapacity + j] << "  ";
            }
            cout << endl;
        }
    }

    /// @brief 获取权重
    /// @return val 权重
    bool getValueFromMatrix(int row, int col, int &val)
    {
        if (row < 0 || row >= m_iCapacity)
        {
            return false;
        }
        if (col < 0 || col >= m_iCapacity)
        {
            return false;
        }
        // fixme : 邻接矩阵 —— 权重值
        val = m_pMatrix[row * m_iCapacity + col];

        return true;
    }

    /// @brief 设置权重 —— 有向图
    bool setValueToMatrixForDirectedGraph(int row, int col, int val = 1)
    {
        if (row < 0 || row >= m_iCapacity)
        {
            return false;
        }
        if (col < 0 || col >= m_iCapacity)
        {
            return false;
        }

        // 只设置当前节点权重
        m_pMatrix[row * m_iCapacity + col] = val;

        return true;
    }

    /// @brief 设置权重 —— 无向图
    bool setValueToMatrixForUndirectedGraph(int row, int col, int val = 1)
    {
        if (row < 0 || row >= m_iCapacity)
        {
            return false;
        }
        if (col < 0 || col >= m_iCapacity)
        {
            return false;
        }

        // 设置对称节点权重
        m_pMatrix[row * m_iCapacity + col] = val;
        m_pMatrix[col * m_iCapacity + row] = val;

        return true;
    }

    /// @brief 添加顶点
    void addNode(Node *node)
    {
        // assert(node);
        // 顶点集合中，当前顶点的数据进行赋值，从 0 开始
        m_pNodeArray[m_iNodeCount].m_cData = node->m_cData;
        m_iNodeCount++;
    }

    /// @brief 将顶点访问设置默认
    void resetNode()
    {
        for (int i = 0; i < m_iCapacity; i++)
        {
            m_pNodeArray[i].m_bVisited = false;
        }
    }

    /// @brief 最小生成树 —— 普里姆算法
    /// @param index 起始点
    void primTree(int index)
    {
        // 边总数
        int iEdgeCount = 0;
        // 待选边
        vector<Edge> edgeVec;

        // 权重
        int val;

        // 从传入点开始
        vector<int> nodeIndexVec;
        nodeIndexVec.push_back(index);

        // 循环遍历：直到所有边遍历完成
        while (iEdgeCount < m_iCapacity - 1)
        {
            // 查找符合要求关联边（权重不为 0 且目标点未被访问）
            int row = nodeIndexVec.back();
            // 确定起点
            std::cout << m_pNodeArray[row].m_cData << std::endl;
            m_pNodeArray[row].m_bVisited = true;
            // 取值所有边
            for (int i = 0; i < m_iCapacity; i++)
            {
                getValueFromMatrix(row, i, val);
                if (val == 0)
                {
                    continue;
                }
                if (m_pNodeArray[i].m_bVisited)
                {
                    continue;
                }
                Edge edge(row, i, val);
                edgeVec.push_back(edge);
            }

            // 边集合中取最小
            int ret = getMinEdge(edgeVec);
            if (-1 != ret)
            {
                edgeVec[ret].m_bSelected = true;
                m_pEdgeArray[iEdgeCount] = edgeVec[ret];
                // 确定双向连接点和权重
                cout << m_pNodeArray[m_pEdgeArray[iEdgeCount].m_iNodeIndexA].m_cData << " - ";
                cout << m_pNodeArray[m_pEdgeArray[iEdgeCount].m_iNodeIndexB].m_cData << " (";
                cout << m_pEdgeArray[iEdgeCount].m_iWeightValue << ") " << endl;
                iEdgeCount++;
                int iNodeIndex = edgeVec[ret].m_iNodeIndexB;
                // 双向设置已访问
                m_pNodeArray[iNodeIndex].m_bVisited = true;
                nodeIndexVec.push_back(iNodeIndex);
            }
        }
    }

    /// @brief 最小生成树 —— 克鲁斯卡尔算法
    void kruskalTree()
    {
        int val;
        int edgeCount = 0;

        // 存放节点集合数组
        vector<vector<int>> nodeSets;

        // 所有边
        vector<Edge> edgeVec;
        for (int i = 0; i < m_iCapacity; i++)
        {
            for (int j = 0; j < m_iCapacity; j++)
            {
                getValueFromMatrix(i, j, val);
                if (0 == val)
                {
                    continue;
                }
                if (m_pNodeArray[i].m_bVisited)
                {
                    continue;
                }
                Edge edge(i, j, val);
                edgeVec.push_back(edge);
            }
        }

        // 所有边中取出最小生成树的边
        while (edgeCount < m_iCapacity - 1)
        {
            // 从边集合中取出最小边
            int retIndex = getMinEdge(edgeVec);
            if (-1 != retIndex)
            {
                edgeVec[retIndex].m_bSelected = true;
                // 最小边连接点
                int nodeAIndex = edgeVec[retIndex].m_iNodeIndexA;
                int nodeBIndex = edgeVec[retIndex].m_iNodeIndexB;

                // 找出对应点所在集合
                bool nodeAInSet = false;
                bool nodeBInSet = false;
                int nodeAInSetLabel = -1;
                int nodeBInSetLabel = -1;
                for (int i = 0; i < (int)nodeSets.size(); i++)
                {
                    nodeAInSet = isInSet(nodeSets[i], nodeAIndex);
                    if (nodeAInSet)
                        nodeAInSetLabel = i;
                }
                for (int i = 0; i < (int)nodeSets.size(); i++)
                {
                    nodeBInSet = isInSet(nodeSets[i], nodeBIndex);
                    if (nodeBInSet)
                        nodeBInSetLabel = i;
                }

                // 不同点不同处理
                if (nodeAInSetLabel == -1 && nodeBInSetLabel == -1) // 两点均不在集合中
                {
                    vector<int> vec;
                    vec.push_back(nodeAIndex);
                    vec.push_back(nodeBIndex);
                    nodeSets.push_back(vec);
                }
                else if (nodeAInSetLabel == -1 && nodeBInSetLabel != -1) // 起点不在集合中
                {
                    nodeSets[nodeBInSetLabel].push_back(nodeAIndex);
                }
                else if (nodeAInSetLabel != -1 && nodeBInSetLabel == -1) // 终点不在集合中
                {
                    nodeSets[nodeAInSetLabel].push_back(nodeBIndex);
                }
                else if (nodeAInSetLabel != -1 && nodeBInSetLabel != -1 && nodeAInSetLabel != nodeBInSetLabel) // 都在集合中
                {
                    // 存在且标记不同，不形成回路
                    nodeSets[nodeAInSetLabel].insert(nodeSets[nodeAInSetLabel].end(), nodeSets[nodeBInSetLabel].begin(), nodeSets[nodeBInSetLabel].end());

                    for (int k = nodeBInSetLabel; k < (int)nodeSets.size() - 1; k++)
                    {
                        nodeSets[k] = nodeSets[k + 1];
                    }
                }
                // 存在且标记相同，形成回路，不可插入
                else if (nodeAInSetLabel != -1 && nodeBInSetLabel != -1 && nodeAInSetLabel == nodeBInSetLabel)
                {
                    continue;
                }

                m_pEdgeArray[edgeCount] = edgeVec[retIndex];
                edgeCount++;

                cout << m_pNodeArray[edgeVec[retIndex].m_iNodeIndexA].m_cData << " - ";
                cout << m_pNodeArray[edgeVec[retIndex].m_iNodeIndexB].m_cData << " (";
                cout << edgeVec[retIndex].m_iWeightValue << ") " << endl;
            }
        }
    }

};
