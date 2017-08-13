
-------------------------------------------
--  c++模块里面导出的函数，在这里做lua的包装
--
-------------------------------------------

function lottery_box_lottery( lbobj, lottery_type , gold_num , gold_fish)
    LotteryBox.LotteryBoxCallBack(lbobj, lottery_type , gold_num , gold_fish)
end

