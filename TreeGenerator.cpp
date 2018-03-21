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

Transform* TreeGenerator::generateTree(int levels)
{
	if (levels)
	{
		return createTree(levels, levels);
	}
	return NULL;
}

Transform* TreeGenerator::createTree(int level, int maxlevel)
{
	Transform* newBranch = new Transform();
	Transform* branchTransform = new Transform();
	newBranch->translate(0, POTENTIAL(TRUNKHEIGHT, level, maxlevel), 0);
	branchTransform->scaleAbs(POTENTIAL(TRUNKWIDTH, level, maxlevel), POTENTIAL(TRUNKHEIGHT, level, maxlevel), POTENTIAL(TRUNKWIDTH, level, maxlevel));
	branchTransform->addChild(&branch);
	newBranch->addChild(branchTransform);
	if(--level)
	{
		//left branch
		Transform* subBranch = createTree(level, maxlevel);
		subBranch->translate(0, POTENTIAL(TRUNKWIDTH, level + 1, maxlevel) / 2.0f, 0);
		Transform* subBranchRotate = new Transform();
		subBranchRotate->translate(0, POTENTIAL(TRUNKHEIGHT * 0.6f, level, maxlevel), 0);
		subBranchRotate->rotate(Z_AXIS_VEC, 40 * RANDOMIZE(70, 80));
		subBranchRotate->rotate(Y_AXIS_VEC, 30 * RANDOMIZE(0, 100));
		subBranchRotate->addChild(subBranch);
		newBranch->addChild(subBranchRotate);

		//middle branch
		subBranch = createTree(level, maxlevel);
		subBranch->translate(0, POTENTIAL(TRUNKHEIGHT, level + 1, maxlevel) / 2.0f, 0);
		newBranch->addChild(subBranch);
		newBranch->addChild(createTree(level, maxlevel));

		//right branch
		subBranch = createTree(level, maxlevel);
		subBranch->translate(0, POTENTIAL(TRUNKWIDTH, level + 1, maxlevel) / 2.0f, 0);
		subBranchRotate = new Transform();
		subBranchRotate->translate(0, POTENTIAL(TRUNKHEIGHT * 0.2f, level, maxlevel), 0);
		subBranchRotate->rotate(Z_AXIS_VEC, -40 * RANDOMIZE(70, 80));
		subBranchRotate->rotate(Y_AXIS_VEC, 30 + 30 * RANDOMIZE(0, 100));
		subBranchRotate->addChild(subBranch);
		newBranch->addChild(subBranchRotate);

		//front right branch
		subBranch = createTree(level, maxlevel);
		subBranch->translate(0, POTENTIAL(TRUNKWIDTH, level + 1, maxlevel) / 2.0f, 0);
		subBranchRotate = new Transform();
		subBranchRotate->translate(0, POTENTIAL(TRUNKHEIGHT * 0.2f, level, maxlevel), 0);
		subBranchRotate->rotate(Z_AXIS_VEC, -40 * RANDOMIZE(70, 80));
		subBranchRotate->rotate(Y_AXIS_VEC, -40 - 30 * RANDOMIZE(0, 100));
		subBranchRotate->addChild(subBranch);
		newBranch->addChild(subBranchRotate);
	}
	else
	{
		Transform* leaves = new Transform;
		leaves->addChild(&world);
		leaves->translate(0, POTENTIAL(TRUNKHEIGHT, level + 1, maxlevel), 0);
		leaves->rotate(X_AXIS_VEC, 90);
		newBranch->addChild(leaves);
	}
	return newBranch;
}
