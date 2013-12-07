
testNode1 = MapNode(46.79, 4.44, 1, 'a');
testNode2 = MapNode(46.8, 4.45, 2, 'b');
testNode3 = MapNode(46.795, 4.44, 3, 'c');

cAdj = cosd( mean([allRoads.boundMin.lat allRoads.boundMax.lat]) );
sNode = testNode3.nodeOnSegment(testNode1, testNode2, cAdj);

hold on
    plot([testNode1.lon testNode2.lon], [testNode1.lat testNode2.lat], 'red')

    plot([testNode3.lon sNode.lon], [testNode3.lat sNode.lat], 'red')
hold off

