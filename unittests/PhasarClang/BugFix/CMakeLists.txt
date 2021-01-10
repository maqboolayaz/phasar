set(BugFixSources
	PhasarClangBugFixTest.cpp
)

test_require_config_file("DOTGraphConfig.json")

foreach(TEST_SRC ${BugFixSources})
	add_phasar_unittest(${TEST_SRC})
endforeach(TEST_SRC)
