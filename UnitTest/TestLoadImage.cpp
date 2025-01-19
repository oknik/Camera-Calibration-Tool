#include "pch.h"
#include "CppUnitTest.h"
#include <fstream>

#include "CalculateCorner.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
	TEST_CLASS(LoadImageTest) {
public:
	// 测试的时候发现CalculateCorner方法是在C:/...下面，为了方便测试我就写成绝对路径了
	CalculateCorner calculator = CalculateCorner();
	// 测试添加、删除、重置文件路径
	TEST_METHOD(AddAndRemovePath) {
		
		// 添加路径
		std::string path1 = "../Resources/Data/OrdinaryCamera/01-20mm/chess (0).JPG";
		std::string path2 = "../Resources/Data/OrdinaryCamera/01-20mm/chess (1).JPG";
		auto paths = this->calculator.GetAllFilePaths();
		Assert::AreEqual(size_t(0), paths.size());

		calculator.AddImagePath(path1);
		calculator.AddImagePath(path2);
		paths = calculator.GetAllFilePaths();
		Assert::AreEqual(size_t(2), paths.size());
		Assert::AreEqual(path1, paths[0]);
		Assert::AreEqual(path2, paths[1]);

		calculator.RemoveImagePath(path1);
		paths = this->calculator.GetAllFilePaths();
		Assert::AreEqual(size_t(1), paths.size());
		Assert::AreEqual(path2, paths[0]);

		calculator.ResetImagePathVector();
		paths = calculator.GetAllFilePaths();
		Assert::AreEqual(size_t(0), paths.size());
	}
	TEST_METHOD(TestCaptureImageFromCamera) {
		std::string testPath = "D:/2024Fall/cv/FinalProject/UnitTest/test_image.jpg";
		bool result = this->calculator.CaptureImageFromCamera(testPath,0);
		Assert::IsTrue(result);
		
		// 检查文件是否存在
		std::ifstream file(testPath);
		Assert::IsTrue(file.good());
		file.close();
	}
	TEST_METHOD(TestChessboardImage) {
		// 使用的测试图片有7行9列的格子，那么BoardSize应为 (9-1, 7-1)
		this->calculator.SetBoardSize(cv::Size(9 - 1, 7 - 1));
		// 测试的时候发现CalculateCorner这个方法是在C:/...下面，为了方便测试我就写成绝对路径了
		std::string ValidPath = "D:/2024Fall/cv/FinalProject/Resources/Data/OrdinaryCamera/01-20mm/chess (0).JPG";
		std::string InValidPath = "D:/2024Fall/cv/FinalProject/UnitTest/test_image.jpg";

		this->calculator.AddImagePath(ValidPath);
		this->calculator.AddImagePath(InValidPath);
		this->calculator.CalculateEveryImageCorners(true, false);
		auto ImageCorner = this->calculator.GetImageCorner();
		Assert::IsTrue(ImageCorner.find(ValidPath) != ImageCorner.end());
		Assert::IsTrue(!ImageCorner[ValidPath].empty());
		Assert::IsTrue(ImageCorner[InValidPath].empty());
	}

	};
}
