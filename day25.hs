input :: (Int, Int)
input = (10441485, 1004920)

findCardIters :: Int -> Int
findCardIters cardPubKey =
    findCardIters' 1 0
    where
        findCardIters' acc iters =
            if acc' == cardPubKey then
                iters'
            else
                findCardIters' acc' iters'
            where
                acc' = (acc * 7) `mod` 20201227
                iters' = iters + 1

transform :: Int -> Int -> Int
transform subjNum iters =
    transform' subjNum iters 1
    where
        transform' subjNum 0 acc = acc
        transform' subjNum iters acc = transform' subjNum (iters - 1) ((acc * subjNum) `mod` 20201227)

answer :: (Int, Int) -> Int
answer (cardPubKey, doorPubKey) = key
    where
        cardIters = findCardIters cardPubKey
        key = transform doorPubKey cardIters

main :: IO ()
main = do
    putStrLn $ show $ answer input
