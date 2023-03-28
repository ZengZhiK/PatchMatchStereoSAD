import os.path
import cv2
import numpy as np
import pfm_util
import open3d as o3d


def disparity_to_points(disparity: np.array, baseline, f, cx, cy, doffs=0):
    points = []
    rows, columns = disparity.shape
    for y in range(rows):
        for x in range(columns):
            if disparity[y, x] > 0:
                Z = baseline * f / (disparity[y, x] + doffs)
                X = Z * (x - cx) / f
                Y = Z * (y - cy) / f
                points.append([X, Y, Z])
    return points


def save_points_to_ply(filepath, points, write_ascii=False):
    point_cloud = o3d.geometry.PointCloud()
    point_cloud.points = o3d.utility.Vector3dVector(points)
    o3d.io.write_point_cloud(filepath, point_cloud, write_ascii=write_ascii)


if __name__ == '__main__':
    root_dir = r'F:\1_Code\2_CPP\PatchMatchStereoSAD\imgs\Speckle\BL150\gdqn\PM3SAD11MF7'
    img_filename = 'left_disparity.pfm'
    disp, _ = pfm_util.readPFM(os.path.join(root_dir, img_filename))

    im0 = cv2.imread(os.path.join(root_dir, '..', 'im0.png'), cv2.IMREAD_GRAYSCALE)

    thres = 280
    mask1 = np.where((disp > thres), 1, 0)
    disp *= mask1

    thresh, mask2 = cv2.threshold(im0, thresh=30, maxval=1, type=cv2.THRESH_BINARY)
    disp *= mask2

    disp_gray = cv2.normalize(disp, None, 0, 255, cv2.NORM_MINMAX, cv2.CV_8UC1)
    cv2.filterSpeckles(disp_gray, newVal=0, maxSpeckleSize=3000, maxDiff=3)
    _, mask3 = cv2.threshold(disp_gray, 0, 1, cv2.THRESH_BINARY)
    disp *= mask3

    suffix = root_dir.split('\\')[-1]
    pfm_util.writePFM(os.path.join(root_dir, f'disp0{suffix}.pfm'), disp)

    if root_dir.find('75') > 0:
        cx, cy, f, b = 584.199, 390.509, 948.694, 75
    else:
        cx, cy, f, b = 578.437805, 392.026093, 946.483093, 150
    print(cx, cy, f, b)
    points = disparity_to_points(disp, b, f, cx, cy)
    save_points_to_ply(rf'{root_dir}/disp0{suffix}.ply', points)

    print('done!')
