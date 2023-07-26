# SNR-yolo-moos-integration
OverView: Using yolo object detection and homography to localize people overboard for automonous USV's search and rescue missions with integration in moos-ivp middleware

# Custom Maritime Search and Rescue Dataset

This repository contains a custom Maritime Search and Rescue (SNR) dataset, specifically curated for object detection tasks using the YOLOv5 model. Additionally, the repository includes a MOOS-ivp wrapper for YOLOv5 and for OpenCV homography to facilitate seamless integration with the MOOS-ivp framework.

## Dataset

The custom Maritime SNR dataset comprises annotated images and corresponding object labels necessary for training and evaluating an object detection model. The dataset focuses on maritime scenarios and contains a variety of objects commonly encountered during SNR operations, such as lifebuoys, life jackets, boats, and more.

Please refer to the `dataset/` directory for more information on the dataset structure and instructions on how to use it for training your YOLOv5 model.

## YOLOv5 Model for Object Detection

YOLOv5 is a popular real-time object detection model known for its efficiency and accuracy. In this repository, we provide a pre-trained YOLOv5 model that has been fine-tuned using the custom Maritime SNR dataset. You can use this model to perform object detection on new maritime SNR images or fine-tune it further on your specific dataset.

Instructions on how to use the YOLOv5 model for inference and training can be found in the `yolov5/` directory.

## MOOS-ivp Wrapper for YOLOv5

The MOOS-ivp wrapper for YOLOv5 allows you to seamlessly integrate the YOLOv5 object detection capabilities into your MOOS-ivp applications or systems. By using this wrapper, you can leverage the power of YOLOv5 for real-time object detection in maritime SNR scenarios within the MOOS-ivp framework.

Details on how to use the MOOS-ivp wrapper for YOLOv5 can be found in the `moos_ivp_wrapper/` directory.

## Contributions

We welcome contributions from the community to enhance the dataset, improve the YOLOv5 model, or extend the MOOS-ivp wrapper for YOLOv5. Please refer to the `CONTRIBUTING.md` file to learn about the contribution guidelines.

## License

The code and dataset in this repository are provided under the [MIT License](LICENSE).

---

*Note: Below is the code version of the README. Please use it for your repository's README.md file.*

```markdown
# Custom Maritime Search and Rescue Dataset

This repository contains a custom Maritime Search and Rescue (SNR) dataset, specifically curated for object detection tasks using the YOLOv5 model. Additionally, the repository includes a MOOS-ivp wrapper for YOLOv5 to facilitate seamless integration with the MOOS-ivp framework.

## Dataset

The custom Maritime SNR dataset comprises annotated images and corresponding object labels necessary for training and evaluating an object detection model. The dataset focuses on maritime scenarios and contains a variety of objects commonly encountered during SNR operations, such as lifebuoys, life jackets, boats, and more.

Please refer to the `dataset/` directory for more information on the dataset structure and instructions on how to use it for training your YOLOv5 model.

## YOLOv5 Model for Object Detection

YOLOv5 is a popular real-time object detection model known for its efficiency and accuracy. In this repository, we provide a pre-trained YOLOv5 model that has been fine-tuned using the custom Maritime SNR dataset. You can use this model to perform object detection on new maritime SNR images or fine-tune it further on your specific dataset.

Instructions on how to use the YOLOv5 model for inference and training can be
