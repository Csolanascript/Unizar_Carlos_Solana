import BinaryTree

tree0 = tree 1 (tree 2 (leaf 3) (leaf 4)) (tree 5 empty (leaf 6))
tree1 = tree "R" (tree "HI" (leaf "NII") (leaf "NID")) (tree "HD" (leaf "NDI") (leaf "NDD"))

main = do
  test_tree tree0
  test_tree tree1
  test_list [3, 2, 2, 5, 1, 4, 4]
  test_list [2, 5, 1, 3, 2, 4, 4]
  test_list ["Adolfo","Diego","Juan","Pedro","Tomas"]
  test_list [1..16]
  test_list [3,3,3,3,3]
  test_list [1,3,3,3,5]

  print $ between (buildBalanced [3, 2, 2, 5, 1, 4, 4]) 3 4
  print $ between (buildBalanced ["Adolfo","Diego","Juan","Pedro","Tomas"]) "B" "Q"

test_tree t = do
  putStrLn "--------------------------------------------"
  print $ size t
  print t
  print $ preorder t
  print $ postorder t
  print $ inorder t
  putStrLn "--------------------------------------------"

test_list l = do
  putStrLn "============================================"
  print l
  putStrLn "build"
  test_tree t0
  putStrLn "balance"
  test_tree t1
  putStrLn "build-balanced"
  test_tree t2
  putStrLn "============================================"
  where
    t0 = build l
    t1 = balance t0
    t2 = buildBalanced l
