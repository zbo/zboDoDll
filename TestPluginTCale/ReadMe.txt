(done)1. C++ UTest integration
	1.1 gtest online doc, https://github.com/google/googletest/blob/master/googletest/docs/primer.md
	1.2 gtest ignore test keyword,  
2. Refactory const to enum
3. Logic to arrange DD GD
4. Add period in to input array



2019-12-5:暂时用如下规则产生笔：
			1. 顶和底的第二K不能有重合。
			2. 顶和底顶1，3K不能打穿底和顶的最低点和最高点，但是允许类1，3K触碰第2K。

2019-12-6：开始做笔到线段的升级，线段主框架完成

2019-12-7: 1. 完成 DE001 去除特征序列中重复元素
           2. 完成 DE002 002947 给第一段开始补上一个特征序列分型，使得可以画出第一段
		   3. DE003 修复603386，29.56应该是一个顶
		   4. DE004 修复603387，段和笔重合了
		   5. DE005 TZXL顶后顶，底后底问题消除