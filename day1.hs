nat = [1..]

parse content = map read $ lines content

answer input =
    head [
        x * y |
        (x, xi) <- zip input nat,
        y <- take xi input,
        x + y == 2020
    ]

main = do
    content <- readFile "day1.txt"
    let input = parse content
    putStrLn $ show $ answer input
