#
# Be sure to run `pod lib lint MJLog.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see https://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'MJLog'
  s.version          = '0.1.0'
  s.summary          = 'A short description of MJLog.'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
TODO: Add long description of the pod here.
                       DESC

  s.homepage         = 'https://github.com/Musjoy/MJLog'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'xdong90' => 'dong.xia@musjoy.com' }
  s.source           = { :git => 'https://github.com/Musjoy/MJLog.git', :tag => "v-#{s.version}" }
  # s.social_media_url = 'https://twitter.com/<TWITTER_USERNAME>'

  s.ios.deployment_target = '8.0'

  s.source_files = 'MJLog/Classes/**/*'
  s.vendored_frameworks = 'MJLog/Framework/mars.framework'
  s.resource = 'MJLog/Resource/MJLogStrings.bundle', 'MJLog/Resource/decode_mars_nocrypt_log_file.py'
  
  s.user_target_xcconfig = {
    'GCC_PREPROCESSOR_DEFINITIONS' => 'MODULE_LOG'
  }
  
  s.dependency 'ModuleCapability'
  s.frameworks = 'UIKit', 'SystemConfiguration', 'CoreTelephony'
  s.libraries = 'resolv.9','z'
  s.dependency 'SSZipArchive', '~> 2.2.2'

  # s.resource_bundles = {
  #   'MJLog' => ['MJLog/Assets/*.png']
  # }

  # s.public_header_files = 'Pod/Classes/**/*.h'
  # s.dependency 'AFNetworking', '~> 2.3'
end
