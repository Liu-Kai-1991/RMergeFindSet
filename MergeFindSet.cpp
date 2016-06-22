#include <Rcpp.h>
#include <unordered_map>
#include <memory>
#include <list>
using namespace Rcpp;
// Enable C++11 via this plugin (Rcpp 0.10.3 or later)
// [[Rcpp::plugins(cpp11)]]

// [[Rcpp::export]]
int useAuto() {
    auto val = 42;		// val will be of type int
    return val;
}


class Node
{
public:
    Node():Parent(this),rank(0){}
    Node* getParent()
    {
        if (Parent == this) return this;
        else return Parent = Parent->getParent();
    }
    static void merge(Node* x, Node* y)
    {
        Node* PreX = x->getParent();
        Node* PreY = y->getParent();
        if (PreX!=PreY)
        {
            if (PreX->rank>PreY->rank) PreY->Parent = PreX;
            else 
            {
                PreX->Parent = PreY;
                if (PreX->rank==PreY->rank)
                    PreY->rank++;
            }
        }
    }
private:
    Node* Parent;
    int rank;
};

class MergeFindSet
{
public:
    MergeFindSet(void){}
    SEXP greet(SEXP x);
    DataFrame getGroup();
    void insert(SEXP x);
    void combine(SEXP x, SEXP y);
    ~MergeFindSet()
    {
        for (auto i = NodeList.begin();i!=NodeList.end();i++)
            delete (*i);
        Rcout<<"Garbage Cleaned\n";
    }
private:
    std::unordered_map<std::string, Node*> ItemList;
    std::list<Node*> NodeList;
    Node* insert_(SEXP x);
};

SEXP MergeFindSet::greet(SEXP x)
{ 
    return x; 
}

void MergeFindSet::combine(SEXP x, SEXP y)
{
    auto NodeX = insert_(x);
    auto NodeY = insert_(y);
    Node::merge(NodeX, NodeY);
}
    
Node* MergeFindSet::insert_(SEXP x)
{
    std::string strx = as<std::string>(x);
    auto got = ItemList.find(strx);
    if (got == ItemList.end())
    {
        NodeList.push_back(new Node());
        ItemList[strx] = NodeList.back();
        return ItemList[strx];
    }
    else return ItemList[strx];
}

void MergeFindSet::insert(SEXP x)
{
    insert_(x);
}

DataFrame MergeFindSet::getGroup()
{
    CharacterVector ContainerIndividual;
    IntegerVector ContainerGroup;
    std::unordered_map<Node*, int> GroupList;
    int GroupIndex = 0;
    for (auto i = ItemList.begin();i!=ItemList.end();i++)
    {
        ContainerIndividual.push_back(i->first);
        auto x = i->second;
        x = x->getParent();
        auto got = GroupList.find(x);
        if (got == GroupList.end())
        {
            GroupList[x] = ++GroupIndex;
            ContainerGroup.push_back(GroupIndex);
        } else ContainerGroup.push_back(got->second);
    }
    return DataFrame::create(_["Individual"]= ContainerIndividual, _["GroupID"]= ContainerGroup);
}

RCPP_MODULE(play)
{
    using namespace Rcpp ;
    class_<MergeFindSet>( "MergeFindSet" )
    // expose the default constructor
    .constructor()    
    .method( "greet", &MergeFindSet::greet , "get the message" )
    .method( "insert", &MergeFindSet::insert , "insert a single element as a seperate set" )
    .method( "getGroup", &MergeFindSet::getGroup , "get group ID" )
    .method( "combine", &MergeFindSet::combine , "combine two element into the same set" )
    .method( "combine", &MergeFindSet::combine , "combine two element into the same set" )
    ;
}