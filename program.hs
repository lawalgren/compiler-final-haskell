solve :: Int -> [String] -> String
solve index [] = "makes sense"
solve index (next:rest) = if next == show index || next == "mumble"
  then solve (index + 1) rest ++ ""
  else "something is fishy"

main :: Void -> Void
main = do
 getLine
 input <- getLine
 let inputlist = words input
 let answer = solve 1 inputlist
 putStrLn answer
