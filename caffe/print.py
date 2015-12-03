#!/usr/bin/env python
import sys
import yaml
import pprint

def print_model(conf):
    pp = pprint.PrettyPrinter()
    pp.pprint(conf)

    sys.path.insert(0, conf['caffeRoot'] + 'python')
    import caffe

    net = caffe.Net(conf['proto'], conf['model'], caffe.TEST)

    print("\n======================================================")
    print("blobs: {}\nparams: {}".format(net.blobs.keys(), net.params.keys()))
    print("======================================================")
    
    for param in conf['params']:
        print '{}:'.format(param)
        print 'weights are {} dimensional'.format(net.params[param][0].data.shape)
        shape = net.params[param][0].data.shape
        for i in range(shape[0]):
            for j in range(shape[1]):
                print '{}-{}:'.format(i+1, j+1)
                print net.params[param][0].data[i][j]

        print '\nbiases are {} dimensional'.format(net.params[param][1].data.shape)
        print '{}'.format(net.params[param][1].data)
        print("======================================================")
    
def main(argv):
    if (len(argv) < 1):
        print 'Usage: ' + sys.argv[0] + ' conf.yml'
        sys.exit(1)

    with open(argv[0], 'r') as f:
        print_model(yaml.load(f))

if __name__ == "__main__":
    main(sys.argv[1:])
