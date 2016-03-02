#!/usr/bin/env python
import sys
import yaml
import pprint

def combine_model(conf):
    pp = pprint.PrettyPrinter()
    pp.pprint(conf)
    
    sys.path.insert(0, conf['caffeRoot'] + 'python')
    import caffe

    net = []
    for i, source in enumerate(conf['source']):
        net.append(caffe.Net(source['proto'], source['model'], caffe.TEST))
    target = conf['target']
    targetNet = net[target['id']]

    for targetName in conf['replace']:
        source = conf['replace'][targetName]
        print targetName + ': ' + source

        if targetName not in targetNet.params:
            print '    No layer ' + targetName + ' in model ' + target['id']
        atIdx = source.rindex('@')
        sourceName = source[:atIdx]
        sourceId = int(source[atIdx+1:])
        if sourceId >= len(net) or sourceId < 0:
            print '    No model ' + str(sourceId)
            sys.exit(1)
        if sourceName not in net[sourceId].params:
            print '    No layer ' + sourceName + ' in model ' + str(sourceId)

        for i in [0, 1]:
            if net[sourceId].params[sourceName][i].data.shape != targetNet.params[targetName][i].data.shape:
                print '    Share of ' + targetName + ' and ' + source + ' is different'
            targetNet.params[targetName][i].data[...] = net[sourceId].params[sourceName][i].data

    targetNet.save(target['output'])

def main(argv):
    if (len(argv) < 1):
        print 'Usage: ' + sys.argv[0] + ' conf.yml'
        sys.exit(1)

    with open(argv[0], 'r') as f:
        combine_model(yaml.load(f))

if __name__ == "__main__":
    main(sys.argv[1:])
