#include "TreeGenerator.h"

TreeGenerator::TreeGenerator()
{

	srand(time(NULL));
	branch.init("cylinder2.obj");
	branch.heap = false;
    branch.color = glm::vec3( 0.6f, 0.4f, 0.2f);
    branch.terr = 0.0f;

	leaf.init("sphere.obj");
	leaf.heap = false;
    leaf.color = glm::vec3( 0.2f, 0.8f, 0.2f);
    leaf.terr = 0.0f;
    world.heap = false;
    world.scaleAbs(0.7f, 0.7f, 0.7f);
    world.addChild(&leaf);
    
}

TreeGenerator::~TreeGenerator()
{
}

Transform* TreeGenerator::generateTree(int levels, int which)
{
    std::string X = "FF[+B]F[-&B]F[-^B]FB";
    std::string F = "FF";
    std::string B = "F[+B]F[-&B]F[-^B]FB";
    
    std::string one = "FF[+&B]F[-B]";
    std::string two = "FF[-&B]F F[-^B]";
    std::string tree;
    
    if(which == 1)
    {
        tree = one;
    }
    else if ( which == 2)
    {
        tree = two;
    }
    else
    {
        tree = X;
    }
    
    for (int i = 0; i < levels; ++i)
    {
        for(size_t pos = 0; (pos = tree.find_first_of("FB", pos)) != std::string::npos;)
        {
            switch (tree[pos])
            {
                case 'F':
                {
                    tree.replace(pos, 1, F);
                    pos += F.length();
                    break;
                }
                case 'B':
                {
                    tree.replace(pos, 1, B);
                    pos +=B.length();
                    break;
                }
            }
        }
    }
    if (levels)
    {
        int index = 0;
        return createTree(tree, index, levels, levels);
    }
    return NULL;
}

Transform* TreeGenerator::createTree(std::string rule, int& index, int level, int maxlevel)
{
    if (index >= rule.size())
    {
        return NULL;
    }
    bool done = false;
    Transform* newBranch = new Transform();
    while(!done && index < rule.size())
    {
        switch (rule[index])
        {
            case 'F':
            {
                Transform* branchTransform = new Transform();
                newBranch->translate(0, POTENTIAL(TRUNKHEIGHT, level, maxlevel), 0);
                branchTransform->scaleAbs(POTENTIALW(TRUNKWIDTH, level, maxlevel), POTENTIAL(TRUNKHEIGHT, level, maxlevel), POTENTIALW(TRUNKWIDTH, level, maxlevel));
                branchTransform->addChild(&branch);
                newBranch->addChild(branchTransform);
                done = true;
                break;
            }
            case 'B':
            {
                newBranch->addChild(&leaf);
                newBranch->translate(0, POTENTIAL(TRUNKHEIGHT, level + 1, maxlevel), 0);
                newBranch->rotate(X_AXIS_VEC, 90);
                done = true;
                break;
            }
            case '[':
            {
                Transform* subBranch = createTree(rule, ++index, level - 1, maxlevel);
                newBranch->addChild(subBranch);
                done = true;
                break;
            }
            case '+':
            {
                newBranch->rotate(Z_AXIS_VEC, Zturn + PLUSMINUS(20));
                if (isalpha(rule[index + 1]))
                {
                    Transform* subBranch = createTree(rule, ++index, level, maxlevel);
                    subBranch->translate(0, POTENTIALW(TRUNKWIDTH, level + 1, maxlevel) / 2.0f, 0);
                    newBranch->addChild(subBranch);
                    return newBranch;
                }
                else
                {
                    ++index;
                }
                break;
            }
            case '-':
            {
                newBranch->rotate(Z_AXIS_VEC, zturn + PLUSMINUS(20));
                if (isalpha(rule[index + 1]))
                {
                    Transform* subBranch = createTree(rule, ++index, level, maxlevel);
                    subBranch->translate(0, POTENTIALW(TRUNKWIDTH, level + 1, maxlevel) / 2.0f, 0);
                    newBranch->addChild(subBranch);
                    return newBranch;
                }
                else
                {
                    ++index;
                }
                break;
            }
            case '&':
            {
                newBranch->rotate(Y_AXIS_VEC, Yturn + PLUSMINUS(20));
                if (isalpha(rule[index + 1]))
                {
                    Transform* subBranch = createTree(rule, ++index, level, maxlevel);
                    subBranch->translate(0, POTENTIALW(TRUNKWIDTH, level + 1, maxlevel) / 2.0f, 0);
                    newBranch->addChild(subBranch);
                    return newBranch;
                }
                else
                {
                    ++index;
                }
                break;
            }
            case '^':
            {
                newBranch->rotate(Y_AXIS_VEC, yturn + PLUSMINUS(20));
                if (isalpha(rule[index + 1]))
                {
                    Transform* subBranch = createTree(rule, ++index, level, maxlevel);
                    subBranch->translate(0, POTENTIALW(TRUNKWIDTH, level + 1, maxlevel) / 2.0f, 0);
                    newBranch->addChild(subBranch);
                    return newBranch;
                }
                else
                {
                    ++index;
                }
                break;
            }
            default:
            {
                delete newBranch;
                done = true;
                return NULL;
            }
        }
    }
    if(index < rule.size())
    {
        Transform* subBranch = createTree(rule, ++index, level, maxlevel);
        newBranch->addChild(subBranch);
    }
	return newBranch;
}
